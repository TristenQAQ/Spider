#include <iostream>
#include <string>
#include <Windows.h>
#include <queue>
#include <regex>
#include <vector>

using namespace std;
#pragma comment(lib,"ws2_32.lib")

//��ʼץȡ
bool StartCatch(string url);

//http����
class CHttp
{
private:
	string m_host;                                           //����
	string m_object;                                         //��Դ·��
	SOCKET m_socket;                                         //�׽���
public:
	CHttp();
	~CHttp();
	bool Init();                                             //��ʼ������
	bool AnalyseUrl(string url);                             //������ҳ
	bool Connect();                                          //���ӷ�����
	bool FetchHtml(string &html);                            //��ȡ��ҳ
	                                                         //������ҳ
	bool Download(string url, string filename);              //������Դ
	bool UTF8ToANSI(std::string &str1, std::string &str2);   //����ת��
	bool Close();                                            //�ر��׽���
};