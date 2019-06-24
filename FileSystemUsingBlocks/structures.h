struct metaData
{
	unsigned int MagicNumber;
	unsigned int BlockSize;
	unsigned int NoOfBlocks;
	unsigned int EmptyBlocks;
};

typedef metaData MD;

struct fileRecords
{
	char FileName[20];
	unsigned int Size;
	unsigned int BlockCount;
	unsigned int Start;
};

typedef fileRecords FR;

