#include <Koala/VirtualMachine/Tool/LogManager.h>

int main()
{
	// Check machine requirements
	if(sizeof(double) != 8u || 
	   sizeof(double) != sizeof(long long))
	{
		ExitMessage(Koala::Utility::Text::VM_DataSizeMismatchError);
		return 0;
	}

	return 0;
}
