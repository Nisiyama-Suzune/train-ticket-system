void binary_save_test()
{
	QFile file("/home/yanhongyu/Git/Data_structure/load/binary_test.dat");
	if (!file.open(QIODevice::WriteOnly)) {
		std::cout << "File open fail" << std::endl;
		return;
	}
	QDataStream fout(&file);
	QString abc = "ABC";
	fout << qint32(123) << qint32(456) ;
//	fout << abc << x;
}

void binary_load_test()
{
	QFile file("/home/yanhongyu/Git/Data_structure/load/binary_test.dat");
	if (!file.open(QIODevice::ReadOnly)) {
		std::cout << "File open fail" << std::endl;
		return;
	}
	QDataStream fin(&file);
	QString abc;
	qint32 x;
	fin.device() -> seek(sizeof(qint32));
	fin >> x;
	QTextStream cout(stdout);
	cout << x << endl;
	fin.device() -> seek(0);
	fin >> x;
	cout << x << endl;
	fin >> x;
	cout << x << endl;
}

void vector_save_test()
{
	cout << "=========== vector_saving ===============" << endl;
	QFile file("/home/yanhongyu/Git/Data_structure/load/vector_test.dat");
	if (!file.open(QIODevice::WriteOnly)) {
		std::cout << "File open fail" << std::endl;
		return;
	}
	QDataStream fout(&file);
	sjtu::vector<int> a;
//	QVector<qint32> a;
	for (int i = 1; i <= 100; ++i)
		a.push_back(i);
	a.save(fout);
//	fout << a;
}

void vector_load_test()
{
	QFile file("/home/yanhongyu/Git/Data_structure/load/vector_test.dat");
	if (!file.open(QIODevice::ReadOnly)) {
		std::cout << "File open fail" << std::endl;
		return;
	}
	QDataStream fin(&file);
	sjtu::vector<int> a;
	QTextStream cout(stdout);
	cout << "=========== vector_loading ===============" << endl;
	a.load(fin);
	cout << a.size() << endl;
	for (int i = 0; i < a.size(); ++i)
		cout << a[i] << ' ';
	cout << endl;
}

void map_save_test()
{
	QFile file("/home/yanhongyu/Git/Data_structure/load/map_test.dat");
	if (!file.open(QIODevice::WriteOnly)) {
		std::cout << "File open fail" << std::endl;
		return;
	}
	QDataStream fout(&file);
	sjtu::map<QString, int> a;
	a["abc"] = 1;
	a["yhy"] = 213;
	fout << a;
}

void map_load_test()
{
	QFile file("/home/yanhongyu/Git/Data_structure/load/map_test.dat");
	if (!file.open(QIODevice::ReadOnly)) {
		std::cout << "File open fail" << std::endl;
		return;
	}
	QDataStream fin(&file);
	QTextStream cout(stdout);
	sjtu::map<QString, int> a;
	fin >> a;
	for (auto iter = a.begin(); iter != a.end(); ++iter)
		cout << iter->first << ' ' << iter->second << endl;
}

/*
void list_save_test()
{
	QFile file("/home/yanhongyu/Git/Data_structure/load/list_test.dat");
	if (!file.open(QIODevice::WriteOnly)) {
		std::cout << "File open fail" << std::endl;
		return;
	}
	QDataStream fout(&file);
	sjtu::list<QString> a;
	a.push_back("ryw");
	a.push_back("yhy");
	a.save(fout);
//	fout << a;
}

void list_load_test()
{
	QFile file("/home/yanhongyu/Git/Data_structure/load/list_test.dat");
	if (!file.open(QIODevice::ReadOnly)) {
		std::cout << "File open fail" << std::endl;
		return;
	}
	QDataStream fin(&file);
	QTextStream cout(stdout);
	sjtu::list<QString> a;
//	fin >> a;
	a.load(fin);
	for (auto iter = a.begin(); iter != a.end(); ++iter)
		cout << *iter << endl;
}
*/
