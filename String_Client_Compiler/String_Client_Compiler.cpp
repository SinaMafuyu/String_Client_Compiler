#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

/*
	string_client_c.dat�� ������ �Ľ� ��Ģ

	ID���ڿ��� 0a xx 0a xx ������ ���������� ���� 0a xx yy 0x xx (yy�� 80 ������ ����)�� �����ϴ� ��찡 �ִ�.

	yy�� d�� ����ȴ�.

	xx�� 0a�� �ͼ��� �ȵȴ�. 0a0a�� str�� �����̱� �����̴�.

	�ι�° xx (b)�� ID ���ڿ��� �����̴�.

	str���ڿ��� 12 xx ���� ���������� ���� 12 xx zz (zz�� 20������ ����)�� �����ϴ� ��찡 �ִ�.

	str���ڿ����� ������ ���� �� �ֱ� ������ �ݵ�� ������ �ι� �Ǿ����� üũ�ϰ� �����׸����� �����ؾ��Ѵ�.

	12������ xx (c)�� str ���ڿ��� ����(��Ƽ����Ʈ����)�� �����ȴ�.
*/

int main(int argc, char *argv[])
{
	FILE *fp=NULL, *txt = NULL;
	int a, b, c, i, d, e, tmp;
	char id[200], str[2000], sel;
	if (argc == 1 || !strcmp(argv[1], "help"))
	{
		cout << "\tUsage: scc [mode] \"string_client_c.dat Path\" \".txt Path\"" << endl << endl;
		cout << "\tmode:\tcompile \t- .txt to dat" << endl;
		cout << "\t\tdecompile \t- .dat to .txt" << endl;
	}
	else if (argc == 4 && !strcmp(argv[1], "decompile"))
	{
		cout << endl << ">> WARNING: this is DEcompile mode. this will destroy your work data if .txt file Exists.<<" << endl << endl << "Do you want to continue? (y/n) :";

		cin >> sel;

		if (sel != 'y')
		{
			cout << "exits..." << endl;
			return 0;
		}
		try {
			fp = fopen(argv[2], "rb");
			if (!fp)		//������ ���ٸ�
			{
				throw "ERR: No .dat File";
			}
			txt = fopen(argv[3], "wb+");
			if (!txt)		//���ϻ����� �Ұ����ϴٸ�
			{
				throw "ERR: Cannot create new .txt file";
			}

			if (fgetc(fp) != 0x0a)		//������ ������ 0a
			{
				throw "ERR: invaild string_client_c.dat file";
			}

			for (; !feof(fp);)
			{
				a = 0, b = 0, c = 0, d=0, e=0, i=0;
				a=fgetc(fp);	//0a ������ ���ڴ� a�� �޴´�
				if (a == 0x0a)
					throw "ERR: invaild string_client_c.dat file";
				d = fgetc(fp);	//���� 0a�� �ٷ� ���� �ʰ� 80������ ���ڰ� ������ ��찡 �ִ�. �� ���� d�� ����
				if (d != 0x0a)
				{
					if (d >= 0x80 || fgetc(fp) != 0x0a)
					{
						throw "ERR: invaild string_client_c.dat file";
					}
				}
				b = fgetc(fp);	//0a ���� ���ڸ� �޴´�
				for (i=0; !feof(fp);i++)
				{
					if (i >= 200)
						throw "ERR: Need more ID string buffer in source code";
					id[i] = fgetc(fp);
					if (id[i] == 0x12)	//12�� ������ str���ڿ���
					{
						id[i] = 0;
						c = fgetc(fp);	//12 �������ڴ� c��
						e = fgetc(fp);	//���� 20������ ���ڰ� �߰��� �ۼ��ȴ�.
						if (e >= 0x20)
						{
							fseek(fp, -1, SEEK_CUR);
							e = 0;
						}
						break;
					}
					else if (id[i] == 0x0a)	//ID���� ������ ����. ��, str�� �����ΰ��.
					{
						id[i] = 0;
						fseek(fp, -1, SEEK_CUR);
						break;
					}
				}
					for (i = 0; !feof(fp); i++)
					{
						if (i >= 2000)
							throw "ERR: Need more str string buffer in source code";
						str[i] = fgetc(fp);
						if (str[i]==EOF)
						{
							str[i] = 0;
							break;
						}
						else if (str[i] == 0x0a)	//�������� �Ǵ�
						{
							tmp = 0;
							if (fgetc(fp) == 0x0a)	//str�� �����ϰ��
							{
								fseek(fp, -1, SEEK_CUR);
								continue;
							}
							tmp = fgetc(fp);
							if (tmp == 0x0a)	//���� �׸��ΰ�
							{
								fgetc(fp);
								if (fgetc(fp) == 0x0a)
								{
									fseek(fp, -4, SEEK_CUR);
									continue;
								}
								str[i] = 0;
								fseek(fp, -4, SEEK_CUR);
								break;
							}
							else if (tmp < 0x80)	//���� �׸��ΰ� �Ǵ�
							{
								if (fgetc(fp) == 0x0a)
								{
									str[i] = 0;
									fseek(fp, -3, SEEK_CUR);
									break;
								}
								fseek(fp, -1, SEEK_CUR);
							}
							fseek(fp, -2, SEEK_CUR);	//������ �ƴϿ��� �׷��� �ٽ� �������� ��� �Է¹ޱ�
						}
					}
					fprintf(txt, "%d %d %d %d %d %s", a, b, c, d, e, id);
					fputc(0x0a, txt);
					fprintf(txt, "%s", str);
					fputc(0x0a, txt);
					fputc(0x0a, txt);
			}
			cout << endl << "decompile finished." << endl;
			cout << endl << "<!--Important--!>" << endl;
			cout << "You should edit string in same byte (this is multibyte char data)" << endl;
			cout << "You shouldn't edit binary number data in file (it will corrupt data.jmp)" << endl;
		}
		catch (char *e) {
			cout << e << endl;
		}
		catch (...) {
		}
		if(fp)
			fclose(fp);
		if(txt)
			fclose(txt);
	}
	else if (argc == 4 && !strcmp(argv[1], "compile"))
	{
		cout << endl << "info: this is compile mode. this will overwite if .dat file Exists." << endl << endl << "Do you want to continue? (y/n) :";

		cin >> sel;

		if (sel != 'y')
		{
			cout << "exits..." << endl;
			return 0;
		}
		try {
			txt = fopen(argv[3], "rb");
			if (!txt)		//������ ���ٸ�
			{
				throw "ERR: No .txt File";
			}
			fp = fopen(argv[2], "wb+");
			if (!fp)		//���ϻ����� �Ұ����ϴٸ�
			{
				throw "ERR: Cannot create new .dat file";
			}

			for (; !feof(txt);)
			{
				a = 0, b = 0, c = 0, d = 0, e = 0, i = 0;
				fscanf(txt, "%d %d %d %d %d %[^\n]", &a, &b, &c, &d, &e, id);
				fgetc(txt);	// 0a����
				cout << a << b << c << d << e << id << endl;
				
				for (int i=0; !feof(txt); i++)
				{
					if (i >= 2000)
						throw "ERR: Need more str string buffer in source code";
					str[i] = fgetc(txt);
					if (str[i] == 0x0a)
					{
						if (fgetc(txt) == 0x0a)
						{
							tmp = fgetc(txt);
							if (feof(txt))
							{
								str[i] = 0;
								break;
							}
							if (tmp >= '0' && tmp <= '9')
							{
								fseek(txt, -1, SEEK_CUR);
								str[i] = 0;
								break;
							}
							fseek(txt, -1, SEEK_CUR);
						}
						fseek(txt, -1, SEEK_CUR);
					}
				}
				fputc(0x0a, fp);
				fputc(a, fp);
				if (d != 0x0a)
					fputc(d, fp);
				fputc(0x0a, fp);
				fputc(b, fp);
				
				for (int i=0;;i++)
				{
					if (id[i] == 0)
						break;
					fputc(id[i], fp);
				}
				if (str[0] != 0)
				{
					fputc(0x12, fp);
					fputc(c, fp);
					if (e != 0)
						fputc(e, fp);
					for (int i = 0;; i++)
					{
						if (str[i] == 0)
							break;
						fputc(str[i], fp);
					}
				}
			}

		}
		catch (char *e) {
			cout << e << endl;
		}
		catch (...) {
		}
		if (fp)
			fclose(fp);
		if (txt)
			fclose(txt);
	}
	else
	{
		cout << "Check Usage: scc -help" << endl;
	}
	return 0;
}