//clock算法
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
	int pageFrames; //clock队列的长度
	vector<struct page*> mem; //内存中的page
							 //vector<int> seqVec; //要访问的序列号
							 //vector<char> seqOp; //对应的操作 read or write
	vector<struct type*> seq; //访问序列

	int id_range[8] = {1,2,3,4,5,5,6,1};  //id
	int id_length = sizeof(id_range) / sizeof(id_range[0]);
	char op_range[8] = {'r','r','w','r','r','w','w','r'};  //与id对应的operation

	int exit_in_mem(struct type*);
};

PageReplacement::PageReplacement()
{
	cout << "请输入clock队列的长度: " << endl;
	cin >> pageFrames;
	for (int i = 0; i< id_length; ++i)
	{
		int id = id_range[i]; //
		//int num = rand() % pageFrames; //随机数
		//char op = num % 2 == 0 ? 'r' : 'w'; //操作类型
		char op = op_range[i];

		struct type* a = (struct type *)malloc(sizeof(struct type));
		a->id = id;
		a->op = op;

		seq.push_back(a);
	}
}

PageReplacement::~PageReplacement()
{
	while (!seq.empty())  //手动清空seq
	{
		struct type *tmp = seq.back();
		seq.pop_back();
		free(tmp);
	}
	while (!mem.empty())
	{
		struct page* page_tmp = mem.back(); //获取mem最后一个元素
		mem.pop_back();  //删除mem中最后一个元素
		free(page_tmp);  //释放malloc申请的空间
	}
}

void PageReplacement::show_seq() {
	cout << "访问的page序列如下：" << endl;
	for (int i = 0; i<this->id_length; ++i)
	{
		cout << this->seq[i]->id << " "<< " " << this->seq[i]->op << endl;
	}
}

void PageReplacement::show_mem()
{
	cout << "内存中的page序列如下：" << endl;
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
		if (mem[i]->id == re->id)  //ID相同，则在内存中
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
		if (num != -1)  //在内存中存在
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
		else {  //不存在
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

			if (mem.size() < pageFrames) //内存不满
			{
				mem.insert(mem.begin(),tmp_page); //插入头部
			}
			else  //内存满
			{
				int pos = clock(); //替换出一个页面
				mem.erase(mem.begin() + pos); //删除POS位置元素
				mem.insert(mem.begin() + pos, tmp_page); //在POS位置插入元素
			}
		}

	}
}

int PageReplacement::clock()
{
	while (1)
	{
		int i,j;
		for (i = 0; i < pageFrames; ++i)  //第一轮clock
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
		for (j = 0; j < pageFrames; ++j)  //第二轮clock
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
