//clock�㷨
#include <iostream>
#include <vector>
#include <malloc.h>
using namespace std;

struct page {
	int id;
	int reference_bit;
	int dirty_bit;
};

struct type {
	int id;
	char op;
};
class PageReplacement
{
public:
	PageReplacement();
	~PageReplacement();
	void show_seq();
	void show_mem();
	void algorithm();
	int clock();
private:
	int pageFrames; //clock���еĳ���
	vector<struct page*> mem; //�ڴ��е�page
							 //vector<int> seqVec; //Ҫ���ʵ����к�
							 //vector<char> seqOp; //��Ӧ�Ĳ��� read or write
	vector<struct type*> seq; //��������

	int id_range[8] = {1,2,3,4,5,5,6,1};  //id
	int id_length = sizeof(id_range) / sizeof(id_range[0]);
	char op_range[8] = {'r','r','w','r','r','w','w','r'};  //��id��Ӧ��operation

	int exit_in_mem(struct type*);
};

PageReplacement::PageReplacement()
{
	cout << "������clock���еĳ���: " << endl;
	cin >> pageFrames;
	for (int i = 0; i< id_length; ++i)
	{
		int id = id_range[i]; //
		//int num = rand() % pageFrames; //�����
		//char op = num % 2 == 0 ? 'r' : 'w'; //��������
		char op = op_range[i];

		struct type* a = (struct type *)malloc(sizeof(struct type));
		a->id = id;
		a->op = op;

		seq.push_back(a);
	}
}

PageReplacement::~PageReplacement()
{
	while (!seq.empty())  //�ֶ����seq
	{
		struct type *tmp = seq.back();
		seq.pop_back();
		free(tmp);
	}
	while (!mem.empty())
	{
		struct page* page_tmp = mem.back(); //��ȡmem���һ��Ԫ��
		mem.pop_back();  //ɾ��mem�����һ��Ԫ��
		free(page_tmp);  //�ͷ�malloc����Ŀռ�
	}
}

void PageReplacement::show_seq() {
	cout << "���ʵ�page�������£�" << endl;
	for (int i = 0; i<this->id_length; ++i)
	{
		cout << this->seq[i]->id << " "<< " " << this->seq[i]->op << endl;
	}
}

void PageReplacement::show_mem()
{
	cout << "�ڴ��е�page�������£�" << endl;
	for (int i = 0; i<this->mem.size(); ++i)
	{
		cout << this->mem[i]->id << " " << " " << this->mem[i]->dirty_bit << " " << this->mem[i]->reference_bit << endl;
	}
}

int PageReplacement::exit_in_mem(struct type* re)
{
	for (int i = 0; i<mem.size(); ++i)
	{
		//TODO
		if (mem[i]->id == re->id)  //ID��ͬ�������ڴ���
		{
			return	i;
		}
	}
	return -1;
}
//TODO
void PageReplacement::algorithm()
{
	for (int i = 0; i<seq.size(); ++i)
	{
		int num = this->exit_in_mem(seq[i]);
		if (num != -1)  //���ڴ��д���
		{
			this->mem[num]->reference_bit = 1;
			if (seq[i]->op == 'w')
			{
				this->mem[num]->dirty_bit = 1;
			}
			else
			{
				this->mem[num]->dirty_bit = 0;
			}
		}
		else {  //������
			struct page* tmp_page = (struct page*)malloc(sizeof(struct page));
			if (seq[i]->op == 'w')
			{
				tmp_page->dirty_bit = 1;
			}
			else
			{
				tmp_page->dirty_bit = 0;
			}
			tmp_page->reference_bit = 1;
			tmp_page->id = seq[i]->id;

			if (mem.size() < pageFrames) //�ڴ治��
			{
				mem.insert(mem.begin(),tmp_page); //����ͷ��
			}
			else  //�ڴ���
			{
				int pos = clock(); //�滻��һ��ҳ��
				mem.erase(mem.begin() + pos); //ɾ��POSλ��Ԫ��
				mem.insert(mem.begin() + pos, tmp_page); //��POSλ�ò���Ԫ��
			}
		}

	}
}

int PageReplacement::clock()
{
	while (1)
	{
		int i,j;
		for (i = 0; i < pageFrames; ++i)  //��һ��clock
		{
			if (mem[i]->dirty_bit == 0 && mem[i]->reference_bit == 0)
			{
				break;
			}
		}
		if (i != pageFrames)
		{
			free(mem[i]);
			return i;
		}
		for (j = 0; j < pageFrames; ++j)  //�ڶ���clock
		{
			if (mem[j]->dirty_bit == 1 && mem[j]->reference_bit == 0)
			{
				break;
			}
			else
			{
				mem[j]->reference_bit = 0;
			}
		}
		if (j != pageFrames)
		{
			free(mem[j]);
			return j;
		}
	}
}
int main()
{
	PageReplacement tmp;
	tmp.show_seq();
	tmp.algorithm();
	tmp.show_mem();
	return 0;
}
