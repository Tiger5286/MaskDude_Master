#pragma once
#include "Vec2.h"
/// <summary>
/// �A�j���[�V�����p�̃N���X�B
/// </summary>
class Animation
{
private:
	int _drawHandle;	// �`��n���h��
	Vec2 _frameSize;	// �t���[���̃T�C�Y�i1�t���[��������̃s�N�Z�����j
	int _maxAnimFrame;		// �A�j���[�V�����̍ő�t���[����
	int _animFrame;			// �A�j���[�V�����̌��݂̃t���[��
	int _oneAnimationFrame; // 1�A�j���[�V����������̃t���[����
	int _frameCount;		// �t���[���J�E���g�i�A�j���[�V�����̍X�V�p�j
	int _scale;		// �g�嗦�i1�ȏ�̐����j
	int _rotate;	// ��]�p�x�i0�`360�̐����j

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

	/*�����o�֐�*/
	void Init(int drawHandle, Vec2 frameSize, int maxFrame,int oneAnimationFrame,int scale);
	void Update();
	void Draw(Vec2 pos,bool isTurn) const;

	int SetRotate(float rotate) { _rotate = rotate; return _rotate; }	// ��]�p�x��ݒ肷��
};

