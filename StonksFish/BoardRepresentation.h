#pragma once
class BoardRepresentation
{
public:
	static int xFromIndex(int index) //index from 0-63 -> file + rank
	{
		return (index % 8); //e.g. 17 % 8 = 1
	}
	static int yFromIndex(int index)
	{
		return 7 - (index - xFromIndex(index)) / 8;
	}
	static int indexFromCoords(int x, int y)
	{
		return (7-y) * 8 + x;
	}
};

