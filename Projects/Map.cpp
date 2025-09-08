#include "Map.h"
#include "dxlib.h"
#include "Game.h"

Map::Map():
	m_terrainHandle(-1),
	m_smallPipeHandle(-1),
	m_bigPipeHandle(-1)
{
}

Map::~Map()
{

}

void Map::Init()
{

}

void Map::End()
{

}

void Map::Update()
{

}

void Map::Draw()
{
	// �}�b�v�`�b�v�̓��e��`��
	for (int y = 0; y < kDrawChipNumY; y++)
	{
		for (int x = 0; x < kDrawChipNumX; x++)
		{
			int drawChipX = x * kDrawChipSize;
			int drawChipY = y * kDrawChipSize;
			int chipNumX = kGraphChipSize * (kMapChip[y][x] % kGraphChipNumX);
			int chipNumY = kGraphChipSize * (kMapChip[y][x] / kGraphChipNumX);
			//DrawRectExtendGraph(drawChipX, drawChipY, drawChipX + kDrawChipSize, drawChipY + kDrawChipSize, chipNumX, chipNumY,
			//	kGraphChipSize, kGraphChipSize, m_terrainHandle, true);
			DrawRectRotaGraph(
				drawChipX + kDrawChipSize / 2, drawChipY + kDrawChipSize / 2, // �`��ʒu
				chipNumX, chipNumY, // �`�挳�̋�`�̍�����W
				kGraphChipSize, kGraphChipSize, // �`�挳�̋�`�̃T�C�Y
				2.0f, // �`��{��
				0.0f, // ��]�p�x
				m_terrainHandle, // �`�悷��摜�̃n���h��
				true); // �����F���g�p���邩�ǂ���
		}
	}
}

void Map::DrawPipe()
{
	// �y�ǂ�`��
	DrawExtendGraph(0, kDrawChipSize * 0.7, kDrawChipSize * 2.8, kDrawChipSize * 5, m_bigPipeHandle, true);
	DrawExtendGraph(kDrawChipSize * 40, kDrawChipSize * 0.7, kDrawChipSize * 37.2, kDrawChipSize * 5, m_bigPipeHandle, true);
	DrawExtendGraph(0, kDrawChipSize * 19.5, kDrawChipSize * 1.5, kDrawChipSize * 21.1, m_smallPipeHandle, true);
	DrawExtendGraph(kDrawChipSize * 40, kDrawChipSize * 19.5, kDrawChipSize * 38.5, kDrawChipSize * 21.1, m_smallPipeHandle, true);
}

void Map::SetHandle(int terrainHandle, int smallPipeHandle, int bigPipeHandle)
{
	m_terrainHandle = terrainHandle;
	m_smallPipeHandle = smallPipeHandle;
	m_bigPipeHandle = bigPipeHandle;
}

bool Map::CheckCollision(Vec2 playerPos, Vec2 playerSize, Vec2& hitPos) const
{
	// �v���C���[�̎l���̍��W���v�Z
	int leftX = static_cast<int>((playerPos.x - playerSize.x / 2) / kDrawChipSize);
	int rightX = static_cast<int>((playerPos.x + playerSize.x / 2) / kDrawChipSize);
	int topY = static_cast<int>((playerPos.y - playerSize.y / 2) / kDrawChipSize);
	int bottomY = static_cast<int>((playerPos.y + playerSize.y / 2) / kDrawChipSize);

	// �z��͈̔̓`�F�b�N(�z��͈̔͊O�A�N�Z�X��h��)
	if (leftX < 0) leftX = 0;
	if (rightX >= kDrawChipNumX) rightX = kDrawChipNumX - 1;
	if (topY < 0) topY = 0;
	if (bottomY >= kDrawChipNumY + 1) bottomY = kDrawChipNumY;

	// �v���C���[���d�Ȃ��Ă���}�b�v�`�b�v���`�F�b�N
	for (int y = topY; y <= bottomY; y++)
	{
		for (int x = leftX; x <= rightX; x++)
		{
			if (IsBlockChip(kMapChip[y][x]))
			{
				// ���������u���b�N�̒��S���W���v�Z
				hitPos.x = x * kDrawChipSize + kDrawChipSize / 2;
				hitPos.y = y * kDrawChipSize + kDrawChipSize / 2;
				return true;  // �����蔻�肠��
			}
		}
	}

	return false;  // �����蔻��Ȃ�
}

bool Map::IsBlockChip(int chipNum)const
{
	bool answer;
	answer = (chipNum == kBlockChipNum || chipNum == kBrickChipNum);
	return answer;
}