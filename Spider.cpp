#include "Spider.h"

string ToLower(string str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] >= 'A'&&str[i] <= 'Z')
		{
			str[i] = str[i] - 'A' + 'a';
		}
		i++;
	}
	return str;
}

CHttp::CHttp()
{
}
CHttp::~CHttp()
{
	WSACleanup();
}
//��ʼ������
bool CHttp::Init()
{
	WSADATA wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd))//windows�׽���
		return false;
	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2)//��λ�ֽڣ���λ�ֽ�
		return false;
	//�����׽���
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
		return false;
	return true;
}

//������ҳ
bool CHttp::AnalyseUrl(string url)
{
	//�ַ�����Сдת����ȫ��תΪСд
	ToLower(url);
	
	if (string::npos == url.find("http://"))//û�ҵ�http://
		return false;
	if (url.length() <= 7)
		return false;
	int pos = url.find('/', 7);//�ӵ�7λ��ʼ��"/"
	if (pos == string::npos)
	{
		m_host = url.substr(7);//�ַ�����ȡ���ӵ�7λ�����
		m_object = "/";
	}
	else
	{
		m_host = url.substr(7, pos-7);//�ڶ���������ʾҪ��ȡ���ٸ��ַ�
		m_object = url.substr(pos);
	}
	if (m_host.empty())
		return false;
	//cout << "��������" << m_host << "\t��Դ·��" << m_object << endl;
	return true;
}
//���ӷ�����
bool CHttp::Connect()
{
	hostent *p = gethostbyname(m_host.c_str());
	if (p == NULL)
		return false;
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(80);//web������Ĭ�϶˿ڣ�80 host to net short �����ֽ���ת��Ϊ�����ֽ���
	memcpy(&sa.sin_addr, p->h_addr, 4);
	
	//���ӷ�����
	if (SOCKET_ERROR == connect(m_socket, (sockaddr*)&sa, sizeof(sa)))
		return false;

	return true;
}
//��ȡ��ҳ
bool CHttp::FetchHtml(string &html)
{
	//Http����
	//GET ����
	string info;  
	info = info + "GET " + m_object + " HTTP/1.1\r\n";
	info = info + "Host: " + m_host + "\r\n";
	info = info + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36\r\n";  
	info = info + "Connection: Close\r\n";
	info = info + "\r\n";
	//���͸�������
	if (SOCKET_ERROR == send(m_socket, info.c_str(), info.length(), 0))
		return false;
	//����
	char ch = 0;
	int nRecv = 0;
	while ((nRecv=recv(m_socket, &ch, sizeof(ch), 0))>0)
	{
		html = html + ch;
	}
	return true;
}
//������ҳ
//������Դ
bool CHttp::Download(string url, string filename)
{
	if (false == Init())//��ʼ������
		return false;
	if (false == AnalyseUrl(url))//����url
		return false;
	if (false == Connect())//���ӷ�����
		return false;
	string info;
	info = info + "GET " + m_object + " HTTP/1.1\r\n";
	info = info + "Host: " + m_host + "\r\n";
	info = info + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36\r\n";
	info = info + "Connection: Close\r\n";
	info = info + "\r\n";

	if (SOCKET_ERROR == send(m_socket, info.c_str(), info.length(), 0))
		return false;
	//���ļ�
	FILE* fp = fopen(filename.c_str(), "wb");
	if (fp == NULL)
		return false;
	//����ͷ��Ϣ
	char ch = 0;
	while (recv(m_socket, &ch, 1, 0))
	{
		if (ch == '\r')
		{
			recv(m_socket, &ch, 1, 0);
			if (ch == '\n')
			{
				recv(m_socket, &ch, 1, 0);
				if (ch == '\r')
				{
					recv(m_socket, &ch, 1, 0);
					if (ch == '\n')
						break;
				}
			}
		}
	}

	//��������
	while (recv(m_socket, &ch, 1, 0))
	{
		fwrite(&ch, 1, 1, fp);
	}
	//�ر��ļ�
	fclose(fp);
	return true;
}
//����ת��
bool CHttp::UTF8ToANSI(std::string &str1, std::string &str2)
{
	//UTF-8תUnicode
	wchar_t *pUnicodeBuffer = new wchar_t[str1.length()];
	memset(pUnicodeBuffer, 0, sizeof(wchar_t)* str1.length());
	MultiByteToWideChar(CP_UTF8, 0, str1.c_str(), str1.length(), pUnicodeBuffer, sizeof(wchar_t)*str1.length());
	//���ַ�ת���ֽ�
	char *pBuffer = new char[wcslen(pUnicodeBuffer)*2];
	memset(pBuffer, 0, sizeof(char)*wcslen(pUnicodeBuffer) * 2);

	WideCharToMultiByte(CP_ACP, 0, pUnicodeBuffer, wcslen(pUnicodeBuffer)*2, pBuffer, wcslen(pUnicodeBuffer)*2, 0, 0);
	str2 = pBuffer;
	delete[] pBuffer;
	delete[] pUnicodeBuffer;
	return true;
}
bool CHttp::Close()
{
	closesocket(m_socket);
	m_host.empty();
	m_object.empty();
	m_socket = 0;
	return true;
}