//---------------------------------------------------------------------------//
// File: FilterFreeframe.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Xml.h"
#include "CtrlVar.h"
#include "FilterFreeframe.h"
#include "GEEngineGrafico.h"

//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterFreeframe::GetVars()
{
  return m_pVars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterFreeframe::SetVar(int iVar, void *pData)
{
  SetParameterStruct SetParam;
  SetParam.ParameterNumber   = iVar;
  memcpy(&SetParam.NewParameterValue, pData, 4);
  m_pPlugMain(FF_SETPARAMETER, (unsigned)&SetParam, m_uInstance);
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterFreeframe::GetVar(int iVar)
{
  plugMainUnion retval = m_pPlugMain(FF_GETPARAMETER, iVar, m_uInstance);
  return (&retval.fvalue);
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterFreeframe::Init(TiXmlElement *pElem)
{
  CFilter::Init(pElem);

  const char *pszFile = SafeString(pElem, "plugin");
  m_hLibrary = LoadLibrary(pszFile);
  if (m_hLibrary)
  {
    m_iWidth    = SafeInteger(pElem, "width",  256);
    m_iHeight   = SafeInteger(pElem, "height", 256);
    m_pPlugMain = (FncPlugMain)GetProcAddress(m_hLibrary, "plugMain");
    plugMainUnion retval = m_pPlugMain ? m_pPlugMain(FF_INITIALISE, 0, 1) : retval;
    if (m_pPlugMain && (retval.ivalue == FF_SUCCESS))
    {
      m_iTextura = g_pGestorMateriales->AddTextura(m_iWidth, m_iHeight, HARD_TEX_FORMAT_A8R8G8B8, false, true, false);
      m_iTextura2= g_pGestorMateriales->AddTextura(m_iWidth, m_iHeight, HARD_TEX_FORMAT_A8R8G8B8, false, false, false, CTextura::SYSTEMMEM);

      VideoInfoStruct VideoInfo;
      VideoInfo.BitDepth    = 2;
      VideoInfo.FrameWidth  = m_iWidth;
      VideoInfo.FrameHeight = m_iHeight;
      VideoInfo.Orientation = FF_ORIENTATION_TL;
      retval = m_pPlugMain(FF_INSTANTIATE, (unsigned)&VideoInfo, 0);
      m_uInstance = retval.ivalue;
      retval = m_pPlugMain(FF_GETNUMPARAMETERS, 0, m_uInstance);
      int iNumParameters = retval.ivalue;
      // Parametros
      m_pVars   = NEW_ARRAY(TCtrlVar, iNumParameters+1);
      for (int i = 0; i < iNumParameters; i++)
      {
        m_pVars[i].iType = TCtrlVar::SLIDER;
        m_pVars[i].iNum  = i;
        retval = m_pPlugMain(FF_GETPARAMETERNAME, i, m_uInstance);
        memset (m_pVars[i].pszName, 0, 16);
        strncpy(m_pVars[i].pszName, retval.svalue, 15);
        m_pVars[i].bLinkable   = true;
        m_pVars[i].iNumOptions = 0;
        // Parametros por defecto
        //retval = m_pPlugMain(FF_GETPARAMETERDEFAULT, i, m_uInstance);
        //SetVar(i, &retval.fvalue);
      }
      m_pVars[i].iType = TCtrlVar::INVALID; // Last
      m_bOk = true;
    }
    if (!m_bOk)
    {
      if (m_pPlugMain == NULL)
        GLOG(("ERR: CFilterFreeframe can't find plugMain function in plugin %s\n", pszFile));
      else if (retval.ivalue == -1)
        GLOG(("ERR: CFilterFreeframe version is not compatible with plugin %s\n", pszFile));
      else
        GLOG(("ERR: CFilterFreeframe can't load plugin %s. Return code = %d\n", pszFile, retval.ivalue));
      FreeLibrary(m_hLibrary);
    }
  }
  else
    GLOG(("ERR: CFilterFreeframe can't load library %s\n", pszFile));

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterFreeframe::End()
{
  if (IsOk())
  {
    FreeLibrary(m_hLibrary);
    g_pGestorMateriales->RemoveTextura(m_iTextura);
    g_pGestorMateriales->RemoveTextura(m_iTextura2);
    DISPOSE_ARRAY(m_pVars);
    CFilter::End();
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterFreeframe::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFilterFreeframe::Draw(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  pDD->ApplyBasicShader();
  pDD->EndScene();

  // Copiar la textura
  CTextura    *pTexSrc = g_pGestorMateriales->GetTextura(pRenderTgt->iTexture);
  CTextura    *pTexDst = g_pGestorMateriales->GetTextura(m_iTextura);
  CTextura    *pTexDst2= g_pGestorMateriales->GetTextura(m_iTextura2);
  RECT RectSrc = {0,0, pTexSrc->GetWidth(),pTexSrc->GetHeight()};
  RECT RectDst = {0,0, pTexDst->GetWidth(),pTexDst->GetHeight()};

  // Stretch o copy directamente si el tama�o es el mismo
  if (m_iWidth != pTexSrc->GetWidth() || m_iHeight != pTexSrc->GetHeight())
  {
    pD3D->StretchRect(pTexSrc->GetSurfaceD3D(), &RectSrc, pTexDst->GetSurfaceD3D(), &RectDst, D3DTEXF_POINT);
    pD3D->GetRenderTargetData(pTexDst->GetSurfaceD3D(), pTexDst2->GetSurfaceD3D());
  }
  else
    pD3D->GetRenderTargetData(pTexSrc->GetSurfaceD3D(), pTexDst2->GetSurfaceD3D());

  // Llamar el process del filtro
  TSurfaceDesc Desc;
  if (pTexDst2->Lock(NULL, Desc))
  {
    m_pPlugMain(FF_PROCESSFRAME, (unsigned)Desc.pBits, m_uInstance);
    pTexDst2->Unlock();
  }
  // Actualizar textura
  pD3D->UpdateSurface(pTexDst2->GetSurfaceD3D(), NULL, pTexDst->GetSurfaceD3D(), NULL);
  // Y pintar
  pDD->SetRenderTarget(-1);
  pDD->BeginScene();
  ApplyMode(pDD, pRenderTgt, fAlphaGlobal);
  g_pGestorMateriales->SetTextura(m_iTextura, 0);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  DrawQuad (pDD);
}
