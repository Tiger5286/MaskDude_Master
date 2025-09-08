#include "Animation.h"
#include "DxLib.h"

void Animation::Init(int drawHandle, Vec2 frameSize, int maxAnimFrame,int oneAnimationFrame,int scale)
{
	// メンバ変数の初期化
	_drawHandle = drawHandle;
	_frameSize = frameSize;
	_maxAnimFrame = maxAnimFrame;
	_oneAnimationFrame = oneAnimationFrame;
	_scale = scale;
}

void Animation::Update()
{
	// フレーム数の更新
	_frameCount++;

	// アニメーションフレームの更新
	if (_frameCount > _oneAnimationFrame)
	{
		// フレーム数が指定のフレーム数を超えたらアニメーションフレームを更新
		_animFrame++;
		// フレームが最大値を超えたらリセット
		if (_animFrame >= _maxAnimFrame)
		{
			_animFrame = 0;
		}
		// フレーム数をリセット
		_frameCount = 0;
	}
}

void Animation::Draw(Vec2 pos,bool isTurn) const
{
	//animationの描画
	DrawRectRotaGraph(
		pos.x, pos.y,// 描画位置
		_animFrame * _frameSize.x, 0,// 描画元の矩形の左上座標
		_frameSize.x, _frameSize.y,// 描画元の矩形のサイズ
		_scale, // 描画倍率
		_rotate, // 回転角度
		_drawHandle,
		TRUE, isTurn);
	

	//animationの描画
	//DrawRectRotaGraph(
	//	100, 100,// 描画位置
	//	0, 0,// 描画元の矩形の左上座標
	//	100, 100,// 描画元の矩形のサイズ
	//	_scale, // 描画倍率
	//	_rotate, // 回転角度
	//	_drawHandle,
	//	TRUE, FALSE);

	//適当な円を描画
	//DrawCircle(pos.x, pos.y, 50, GetColor(255, 0, 0), TRUE);
	//指定の画像を描画
	//DrawGraph(pos.x, pos.y, _drawHandle, TRUE);
}
