#pragma once
#include "Vec2.h"
/// <summary>
/// アニメーション用のクラス。
/// </summary>
class Animation
{
private:
	int _drawHandle;	// 描画ハンドル
	Vec2 _frameSize;	// フレームのサイズ（1フレームあたりのピクセル数）
	int _maxAnimFrame;		// アニメーションの最大フレーム数
	int _animFrame;			// アニメーションの現在のフレーム
	int _oneAnimationFrame; // 1アニメーションあたりのフレーム数
	int _frameCount;		// フレームカウント（アニメーションの更新用）
	int _scale;		// 拡大率（1以上の整数）
	int _rotate;	// 回転角度（0～360の整数）

public:
	Animation() :
		_drawHandle(-1),
		_frameSize(0, 0),
		_maxAnimFrame(0),
		_animFrame(0),
		_oneAnimationFrame(0),
		_frameCount(0),
		_scale(1),
		_rotate(0)
	{
	}

	~Animation() {};

	/*メンバ関数*/
	void Init(int drawHandle, Vec2 frameSize, int maxFrame,int oneAnimationFrame,int scale);
	void Update();
	void Draw(Vec2 pos,bool isTurn) const;

	int SetRotate(float rotate) { _rotate = rotate; return _rotate; }	// 回転角度を設定する
};

