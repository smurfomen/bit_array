#include <QCoreApplication>
#include <bit_array>
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	bit_array b(std::vector<int8_t> (3, 0xff));
	b.clear();

	b = std::vector<int8_t> (3, 0xff);
	b &= 0x0f; // all elements &= 0x0f
	b |= 0xf0; // all elements |= 0xf0

	// cleanup all even bits
	for(int i = 0; i < b.size(); ++i) {
		b.set(i, i%2 == 0);
	}

	b = QByteArray(5, 0xaa);



	return a.exec();
}
