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
	// マップチップの内容を描画
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
				drawChipX + kDrawChipSize / 2, drawChipY + kDrawChipSize / 2, // 描画位置
				chipNumX, chipNumY, // 描画元の矩形の左上座標
				kGraphChipSize, kGraphChipSize, // 描画元の矩形のサイズ
				2.0f, // 描画倍率
				0.0f, // 回転角度
				m_terrainHandle, // 描画する画像のハンドル
				true); // 透明色を使用するかどうか
		}
	}
}

void Map::DrawPipe()
{
	// 土管を描画
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
	// プレイヤーの四隅の座標を計算
	int leftX = static_cast<int>((playerPos.x - playerSize.x / 2) / kDrawChipSize);
	int rightX = static_cast<int>((playerPos.x + playerSize.x / 2) / kDrawChipSize);
	int topY = static_cast<int>((playerPos.y - playerSize.y / 2) / kDrawChipSize);
	int bottomY = static_cast<int>((playerPos.y + playerSize.y / 2) / kDrawChipSize);

	// 配列の範囲チェック(配列の範囲外アクセスを防ぐ)
	if (leftX < 0) leftX = 0;
	if (rightX >= kDrawChipNumX) rightX = kDrawChipNumX - 1;
	if (topY < 0) topY = 0;
	if (bottomY >= kDrawChipNumY + 1) bottomY = kDrawChipNumY;

	// プレイヤーが重なっているマップチップをチェック
	for (int y = topY; y <= bottomY; y++)
	{
		for (int x = leftX; x <= rightX; x++)
		{
			if (IsBlockChip(kMapChip[y][x]))
			{
				// 当たったブロックの中心座標を計算
				hitPos.x = x * kDrawChipSize + kDrawChipSize / 2;
				hitPos.y = y * kDrawChipSize + kDrawChipSize / 2;
				return true;  // 当たり判定あり
			}
		}
	}

	return false;  // 当たり判定なし
}

bool Map::IsBlockChip(int chipNum)const
{
	bool answer;
	answer = (chipNum == kBlockChipNum || chipNum == kBrickChipNum);
	return answer;
}