#include "TileMap.h"

TileMap LoadTileMap(const char* filename) noexcept
{
	/*�t�@�C�����J��*/
	std::ifstream file;
	file.open(filename);
	if (!file)
	{
		return {};
	}

	TileMap tileMap;
	tileMap.data.reserve(17 * 110);/*�K���ȗe�ʂ�\��*/

	/*�t�@�C����ǂݍ���*/
	while (!file.eof())
	{
		/*1�s�ǂݍ���*/
		std::string line;
		std::getline(file, line);
		if (line.size() <= 0)
		{
			continue;/*�f�[�^�̂Ȃ��s�𖳎�*/
		}
		line.push_back(',');/*�I�[��ǉ�*/

		/*�J���}��؂蕶����𐔒l�ɕϊ�*/
		tileMap.sizex = 0;
		const char* start = line.data();
		const char* end = start + line.size();
		for (const char* p = start; p != end; ++p)
		{
			if (*p == ',') {
				const int n = atoi(start);
				tileMap.data.push_back(n);
				start = p + 1;/*���̃J���}��؂���w���悤�ɍX�V*/
				++tileMap.sizex;
			}
		}
		++tileMap.sizey;
	}

	return tileMap;
}