#include <iostream>
#include <QTextStream>
#include <QDataStream>
#include <QFile>
#include <QVector>
#include "vector.hpp"
#include "map.hpp"
#include "memory.hpp"
#include <QFile>
//#include "list.hpp"

using namespace std;


extern void test4();

int main()
{
//	test4();
	using namespace sjtu;
	for (int i = 0; i < 100; ++i) {
		cout << *memory_pool<int>::get_T(i) << ' ';

//		cout << i << ' ';
	}
	cout << endl;
//	for (int i = 0; i < 100; ++i)
//		cout << memory_pool<int>::container[i] << ' ';
//	cout << endl;

	QFile fileout("/home/yanhongyu/Git/Data_structure/load/memory_pool.dat");
	if (!fileout.open(QIODevice::WriteOnly)) {
		throw "NO";
		return 1;
	}
	QDataStream fout(&fileout);
	memory_pool<int>::save(fout);
	fileout.close();

	QFile filein("/home/yanhongyu/Git/Data_structure/load/memory_pool.dat");
	if (!filein.open(QIODevice::ReadOnly)) {
	}
	QDataStream fin(&filein);
	memory_pool<int>::load(fin);

	for (int i = 0; i < 100; ++i)
		cout << memory_pool<int>::container[i] << ' ';
//			 << memory_pool<int>::recycler[i] << ' '
//			 << mem
	return 0;
}
