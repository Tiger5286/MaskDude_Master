#include "DxLib.h"
#include "Game.h"
#include "SceneManager.h"

// �v���O������ WinMain ����n�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �E�C���h�E���[�h�ݒ�
	ChangeWindowMode(true);
	// �E�C���h�E�̃^�C�g���ύX
	SetMainWindowText("MaskDude");
	// ��ʂ̃T�C�Y�ύX
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �`��Ώۂ��o�b�N�o�b�t�@�ɕύX
	SetDrawScreen(DX_SCREEN_BACK);

	// �����Ɏ��s���݂̂̏���������

	SceneManager* pSceneManager = new SceneManager;
	pSceneManager->Init();	// �V�[���}�l�[�W���̏�����

	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n���Ԃ��擾
		LONGLONG start = GetNowHiPerformanceCount();

		// �O�̃t���[���ɕ`�悵�����e���N���A����
		ClearDrawScreen();

		// �����ɃQ�[���̏���������

		pSceneManager->Update();	// �V�[���}�l�[�W���̍X�V
		pSceneManager->Draw();		// �V�[���}�l�[�W���̕`��

		// esc�L�[�ŏI��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// �`�悵�����e����ʂɔ��f����
		ScreenFlip();

		// �t���[�����[�g��60�ɌŒ�
		while (GetNowHiPerformanceCount() - start < 16667)
		{

		}
	}

	pSceneManager->End();

	delete pSceneManager;
	pSceneManager = nullptr;	// ����ăA�N�Z�X���Ȃ��悤�ɂ���

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}