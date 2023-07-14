#include <common.h>
#include <Menu.h>
#include <Receiver.h>
#include <Dictionary.h>

// int main(int argc, char const *argv[])
int main(void)
{
	std::thread menu{ Menu{} };
	std::thread receiver{ Receiver{} };
	std::thread dictionary{ Dictionary{} };

	receiver.join();
	menu.join();
	dictionary.join();

	return 0;
}
