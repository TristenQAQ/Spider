#include "Spider.h"
int main()
{
	//��ʼ����
	cout << "\t\t\t\t************************************************" << endl;
	cout << "\t\t\t\t-           ��ӭʹ��������������ϵͳ           -" << endl;
	cout << "\t\t\t\t************************************************" << endl;
	cout << "\t\t\t\t������Ҫץȡ��URL���ӣ�" << endl;

	string url;
	cin >> url;
	//�����ļ���  Windows API
	//���·��
	CreateDirectory(L"./image", NULL);

	//��ʼץȡ
	StartCatch(url);

	return 0;
}
bool StartCatch(string url)
{
	queue<string> q;
	q.push(url);
	while (!q.empty())
	{
		//ץȡ
		//ȡ��URL
		string currentUrl = q.front();
		q.pop();

		CHttp http;
		//1.��ʼ������
		http.Init();
		//2.����URL
		http.AnalyseUrl(currentUrl);
		//3.���ӷ�����
		if (false == http.Connect())
			cout << "����ʧ��" << endl;
		else
			cout << "���ӳɹ�" << endl;
		//4.��ȡ��ҳ
		string html;
		string newhtml;
		http.FetchHtml(html);
		http.UTF8ToANSI(html, newhtml);
		//cout << newhtml << endl;

		//5������ҳ
		vector<string> vecImage;
		smatch mat;
		regex rex("http://[^\\s'\"<>()]+");
		string::const_iterator start = html.begin();
		string::const_iterator end = html.end();
		while (regex_search(start, end, mat, rex))
		{
			string per(mat[0].first, mat[0].second);
			//cout << per << endl;
			//ƥ��ͼƬ����
			if (per.find(".jpg") != string::npos || per.find(".png") != string::npos || per.find(".jpeg") != string::npos)
			{
				vecImage.push_back(per);
				cout << per << endl;
			}
			else
			{
				if (per.find("http://www.w3.org/") == string::npos)
					q.push(per);
			}
			start = mat[0].second;
		}
		//6.������Դ
		for (int i = 0; i < vecImage.size(); i++)
		{
			string filename = "F:/VS2013/Visual Studio 2013/Projects/Spider/Spider/image/" +
				vecImage[i].substr(vecImage[i].find_last_of('/') + 1);
			CHttp httpDownload;
			if (false == httpDownload.Download(vecImage[i], filename))
				cout << "����ʧ��" << GetLastError() << endl;
			else
				cout << "���سɹ�" << endl;
		}

	}
	return true;
}