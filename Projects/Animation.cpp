#include "Animation.h"
#include "DxLib.h"

void Animation::Init(int drawHandle, Vec2 frameSize, int maxAnimFrame,int oneAnimationFrame,int scale)
{
	// �����o�ϐ��̏�����
	_drawHandle = drawHandle;
	_frameSize = frameSize;
	_maxAnimFrame = maxAnimFrame;
	_oneAnimationFrame = oneAnimationFrame;
	_scale = scale;
}

void Animation::Update()
{
	// �t���[�����̍X�V
	_frameCount++;

	// �A�j���[�V�����t���[���̍X�V
	if (_frameCount > _oneAnimationFrame)
	{
		// �t���[�������w��̃t���[�����𒴂�����A�j���[�V�����t���[�����X�V
		_animFrame++;
		// �t���[�����ő�l�𒴂����烊�Z�b�g
		if (_animFrame >= _maxAnimFrame)
		{
			_animFrame = 0;
		}
		// �t���[���������Z�b�g
		_frameCount = 0;
	}
}

void Animation::Draw(Vec2 pos,bool isTurn) const
{
	//animation�̕`��
	DrawRectRotaGraph(
		pos.x, pos.y,// �`��ʒu
		_animFrame * _frameSize.x, 0,// �`�挳�̋�`�̍�����W
		_frameSize.x, _frameSize.y,// �`�挳�̋�`�̃T�C�Y
		_scale, // �`��{��
		_rotate, // ��]�p�x
		_drawHandle,
		TRUE, isTurn);
	

	//animation�̕`��
	//DrawRectRotaGraph(
	//	100, 100,// �`��ʒu
	//	0, 0,// �`�挳�̋�`�̍�����W
	//	100, 100,// �`�挳�̋�`�̃T�C�Y
	//	_scale, // �`��{��
	//	_rotate, // ��]�p�x
	//	_drawHandle,
	//	TRUE, FALSE);

	//�K���ȉ~��`��
	//DrawCircle(pos.x, pos.y, 50, GetColor(255, 0, 0), TRUE);
	//�w��̉摜��`��
	//DrawGraph(pos.x, pos.y, _drawHandle, TRUE);
}
