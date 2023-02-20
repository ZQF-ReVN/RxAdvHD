#include "WIP.h"
#include "..\TDA\FileX.h"

#include <Windows.h>


namespace AdvHDStaticLibrary
{
	WIP::WIP() : m_szWIP(0), m_Header({ 0 })
	{

	}

	WIP::~WIP()
	{

	}

	bool WIP::SplitFrame(std::wstring wsWIP)
	{
		if (!OpenWIP(wsWIP)) return false;
		InitIndex();

		size_t szFrame = 0;
		size_t sequence = 0;
		std::wstring folder = m_wsWIP.substr(0, m_wsWIP.size() - 4) + L'/';
		size_t szRead = sizeof(m_Header) + sizeof(WIP_Struct::WIPEntry) * m_vecEntry.size();
		for (auto& iteFrame : m_vecEntry)
		{
			szFrame = iteFrame.uiSize;
			if (m_Header.usBpp == 8) szFrame += 0x400;
			SaveFrame(folder + MakeName(sequence), szRead, szFrame, iteFrame);
			szRead += szFrame;
			sequence++;
		}

		return true;
	}

	bool WIP::MergeFrame(std::wstring wsFolder)
	{
		OpenWIP(wsFolder + L".wip");

		WIP_Struct::WIPEntry frame = { 0 };
		for (size_t iteSequence = 0; iteSequence < m_Header.usFrames; iteSequence++)
		{
			ReadEntry(wsFolder + L'/' + MakeName(iteSequence), frame);
			m_vecEntry.emplace_back(frame);
		}

		std::ofstream oWIP(wsFolder + L".wip.new", std::ios::binary);
		if (!oWIP.is_open()) return false;

		oWIP.write(reinterpret_cast<char*>(&m_Header), sizeof(m_Header));
		for (auto& iteEntry : m_vecEntry)
		{
			oWIP.write(reinterpret_cast<char*>(&iteEntry), sizeof(iteEntry));
		}

		size_t szFrame = 0;
		for (size_t iteSequence = 0; iteSequence < m_Header.usFrames; iteSequence++)
		{
			szFrame = m_vecEntry[iteSequence].uiSize;
			if (m_Header.usBpp == 8) szFrame += 0x400;
			oWIP.write(ReadFrame(wsFolder + L'/' + MakeName(iteSequence), szFrame), szFrame);
		}

		oWIP.flush();

		return true;
	}

	wchar_t* WIP::MakeName(size_t nSequence)
	{
		static wchar_t frameName[0x10] = { 0 };
		swprintf_s(frameName, 0x10, L"%04d.wip", nSequence);
		return frameName;
	}

	bool WIP::OpenWIP(std::wstring wsWIP)
	{
		m_ifsWIP.open(wsWIP, std::ios::binary);
		if (m_ifsWIP.is_open())
		{
			m_wsWIP = wsWIP;
			m_szWIP = static_cast<size_t>(TDA::FileX::GetFileSize(m_ifsWIP));
			m_ifsWIP.read(reinterpret_cast<char*>(&m_Header), sizeof(m_Header));
			CreateDirectoryW((m_wsWIP.substr(0, m_wsWIP.size() - 4)).c_str(), NULL);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool WIP::InitIndex()
	{
		WIP_Struct::WIPEntry frame = { 0 };
		for (size_t iteFrame = 0; iteFrame < m_Header.usFrames; iteFrame++)
		{
			m_ifsWIP.read(reinterpret_cast<char*>(&frame), sizeof(frame));
			m_vecEntry.push_back(frame);
		}

		return true;
	}

	bool WIP::SaveFrame(std::wstring wsFrame, size_t szPos, size_t szFrame, WIP_Struct::WIPEntry& Frame)
	{
		std::ofstream oFrame(wsFrame, std::ios::binary);
		if (!oFrame.is_open()) return false;

		m_ifsWIP.seekg(szPos);
		BufferReSize(szFrame);
		m_ifsWIP.read(m_pBuffer, szFrame);

		WIP_Struct::WIPHeader header = m_Header;
		header.usFrames = 1;
		oFrame.write(reinterpret_cast<char*>(&header), sizeof(header));
		oFrame.write(reinterpret_cast<char*>(&Frame), sizeof(Frame));
		oFrame.write(m_pBuffer, szFrame);

		oFrame.flush();
		return true;
	}
	bool WIP::ReadEntry(std::wstring wsFramePath, WIP_Struct::WIPEntry& Frame)
	{
		std::ifstream iFrame(wsFramePath, std::ios::binary);
		if (!iFrame.is_open()) return false;

		iFrame.seekg(sizeof(m_Header));
		iFrame.read(reinterpret_cast<char*>(&Frame), sizeof(Frame));

		return true;
	}
	char* WIP::ReadFrame(std::wstring wsFramePath, size_t szFrame)
	{
		std::ifstream iFrame(wsFramePath, std::ios::binary);
		if (!iFrame.is_open()) return nullptr;

		iFrame.seekg(sizeof(m_Header) + sizeof(WIP_Struct::WIPEntry));

		BufferReSize(szFrame);
		iFrame.read(m_pBuffer, szFrame);

		return m_pBuffer;
	}
}