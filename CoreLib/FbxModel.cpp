#include "TFbxImporter.h"
bool FbxModel::Release()
{
	Object3D::Release();
	for (int ivb = 0; ivb < m_pVBList.size(); ivb++)
	{
		if (m_pVBList[ivb] != nullptr)
		{
			m_pVBList[ivb]->Release();
		}
	}
	for (int ivb = 0; ivb < m_pVBWeightList.size(); ivb++)
	{
		if (m_pVBWeightList[ivb] != nullptr)
		{
			m_pVBWeightList[ivb]->Release();
		}
	}
	return true;
}
bool FbxModel::SetVertexData()
{
	if (m_pSubVertexList.size() > 0)
	{
		m_pVBList.resize(m_pSubVertexList.size());
	}
	if (m_pSubIWVertexList.size() > 0)
	{
		m_pVBWeightList.resize(m_pSubIWVertexList.size());
	}
	return true;
}
bool FbxModel::CreateVertexBuffer()
{
	if (m_pSubVertexList.size() <= 0) return false;
	HRESULT hr;
	for (int iMtrl = 0; iMtrl < m_pSubVertexList.size(); iMtrl++)
	{
		if (m_pSubVertexList[iMtrl].size() <= 0) continue;

		//gpu메모리에 버퍼 할당(원하는 할당 크기)
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(Vertex) * m_pSubVertexList[iMtrl].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_pSubVertexList[iMtrl].at(0);

		if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &sd, 
			&m_pVBList[iMtrl])))
		{
			return false;
		}
	}
	for (int iMtrl = 0; iMtrl < m_pSubIWVertexList.size(); iMtrl++)
	{
		if (m_pSubIWVertexList[iMtrl].size() <= 0) continue;

		//gpu메모리에 버퍼 할당(원하는 할당 크기)
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(VertexIW) * m_pSubIWVertexList[iMtrl].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_pSubIWVertexList[iMtrl].at(0);

		if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &sd,
			&m_pVBWeightList[iMtrl])))
		{
			return false;
		}
	}

	return true;
}
bool FbxModel::CreateInputLayout()
{

	// 정점쉐이더의 결과를 통해서 정점레이아웃을 생성한다.	
	// 정점버퍼의 각 정점의 어떤 성분을 정점쉐이더에 전달할 거냐
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"INDEX",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,16,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT NumElements = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = m_pd3dDevice->CreateInputLayout(
		layout,
		NumElements,
		m_pVShader->m_pVSCodeResult->GetBufferPointer(),
		m_pVShader->m_pVSCodeResult->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool FbxModel::SetIndexData()
{
	return true;
}

bool FbxModel::PostRender()
{
	UINT StartSlot;
	UINT NumBuffers;
	UINT Strides[2] = { sizeof(Vertex), sizeof(VertexIW) };
	UINT Offsets[2] = { 0, };

	for (int iMtrl = 0; iMtrl < m_pSubVertexList.size(); iMtrl++)
	{		
		if (m_pTextureList.size() > 0 &&m_pTextureList[iMtrl] != nullptr)
		{
			m_pContext->PSSetShaderResources(0, 1, m_pTextureList[iMtrl]->m_pSRV.GetAddressOf());
		}

		ID3D11Buffer* buffer[2] = { m_pVBList[iMtrl], m_pVBWeightList[iMtrl] };
		m_pContext->IASetVertexBuffers(0, 2, buffer,Strides, Offsets);

		if (m_IndexList.size() <= 0)
		{
			m_pContext->Draw(m_pSubVertexList[iMtrl].size(), 0);
		}
		else
		{
			m_pContext->DrawIndexed(m_IndexList.size(), 0, 0);
		}
	}
	return true;
}