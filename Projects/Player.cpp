#include "Player.h"
#include "Dxlib.h"
#include "Game.h"

Player::Player() :
	Character(),
	m_idle(-1),
	m_run(-1),
	m_jump(-1),
	m_fall(-1),
	m_hit(-1),
	m_death(-1),
	m_spawn(-1),
	m_jumpSE(-1),
	m_deathSE(-1),
	m_respawnSE(-1),
	m_snailAttackedSE(-1),
	m_snailDefeatedSE(-1),
	m_state(PlayerState::Idle),
	m_isInvincible(false),
	m_invincibleTime(0),
	m_deathTime(0),
	m_isCanControl(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_pos = kFirstPos;
	m_speed = Vec2(0, 0);
	m_isOnFloor = false;
	m_isAlive = true;
	m_isTurn = false;
	m_state = PlayerState::Idle;
	// �A�j���[�V�����̏�����
	m_noneAnim.Init(-1, Vec2(0, 0), 0, 0, 1.0f); // �����\�����Ȃ��A�j���[�V����
	m_idleAnim.Init(m_idle,Vec2(kGraphPlayerSize, kGraphPlayerSize), kAnimIdleMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_runAnim.Init(m_run, Vec2(kGraphPlayerSize, kGraphPlayerSize), kAnimRunMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_jumpAnim.Init(m_jump, Vec2(kGraphPlayerSize, kGraphPlayerSize), kAnimJumpMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_fallAnim.Init(m_fall, Vec2(kGraphPlayerSize, kGraphPlayerSize), kAnimFallMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_deathAnim.Init(m_death, Vec2(kGraphDeathSize, kGraphDeathSize), kAnimDeathMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	m_spawnAnim.Init(m_spawn, Vec2(kGraphSpawnSize, kGraphSpawnSize), kAnimSpawnMaxFrame, kAnimOneAnimationFrame, kAnimScale);
	// �����A�j���[�V������ݒ�
	m_nowAnim = m_idleAnim;
}

void Player::End()
{
}

void Player::Update()
{

	//��Ԃ̍X�V(�A�j���[�V�����p)
	m_state = UpdateState();

	// �A�j���[�V�����̍X�V
	m_nowAnim.Update();

	if (m_isAlive)
	{
		m_deathTime = 0; // ���S���Ă���̎��Ԃ����Z�b�g

		if (m_isInvincible)
		{
			m_invincibleTime++;
			if (m_invincibleTime > 120) // ���G���Ԃ��I��������
			{
				m_isInvincible = false; // ���G��Ԃ�����
				m_invincibleTime = 0; // ���G���Ԃ����Z�b�g
			}
		}

		// �d��
		Gravity();

		// �������Ă���Ԃ̓W�����v�s��
		if (m_speed.y > kGravity)
		{
			m_isOnFloor = false;
		}

		// ���R�Ɏ~�܂��
		if (m_speed.x > 0)
		{
			m_speed.x -= 0.1f;
		}
		if (m_speed.x < 0)
		{
			m_speed.x += 0.1f;
		}
		if (m_speed.x > -0.1f && m_speed.x < 0.1f)
		{
			m_speed.x = 0.0f;
		}

		if (m_isCanControl)  // ����\�ȏꍇ�̂ݑ�����󂯕t����
		{
			Control(); // ���͂ɂ��ړ�
		}
		
		// ���x�����ƂɎ��̈ʒu���o��
		m_pos += m_speed;
	}
	else
	{
		// ���S���Ă���ꍇ
		m_deathTime++;
		if (m_deathTime == 21)
		{
			m_nowAnim = m_noneAnim;
			m_speed = Vec2(0, 0);
			m_pos = kFirstPos; // �ŏ��̈ʒu�ɖ߂�
		}
		if (m_deathTime == 60 * 2) // 2�b��ɕ����A�j���[�V�������J�n
		{
			PlaySoundMem(m_respawnSE, DX_PLAYTYPE_BACK);
			m_nowAnim = m_spawnAnim;
		}
		if (m_deathTime == 60 * 2 + 21)
		{
			m_isInvincible = true; // ���G��Ԃɂ���
			m_isAlive = true; // �����Ԃ�
		}
	}

	// ���E�̒[�ɍs���Ɣ��Α��Ƀ��[�v
	EdgeWarp();
}

void Player::Control()
{
	// ���͂ɂ��ړ�
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	bool isJumpB = (pad & PAD_INPUT_1);

	// �W�����v
	if (isJumpB && m_isOnFloor)
	{
		PlaySoundMem(m_jumpSE, DX_PLAYTYPE_BACK);
		m_speed.y = kJumpPower;
		m_isOnFloor = false;
	}
	// ���E�ړ�
	if (pad & PAD_INPUT_RIGHT && m_speed.x < kMaxMoveSpeed)
	{
		m_speed.x += kMoveSpeed;
		m_isTurn = false;
	}
	if (pad & PAD_INPUT_LEFT && m_speed.x > -kMaxMoveSpeed)
	{
		m_speed.x -= kMoveSpeed;
		m_isTurn = true;
	}
}

void Player::Draw()
{
	if ((m_invincibleTime % 2) == 0)
	{
		m_nowAnim.Draw(m_pos, m_isTurn);
		m_nowAnim.Draw(Vec2(m_pos.x + Game::kScreenWidth, m_pos.y), m_isTurn); // ��ʂ̉E���ɂ��`��
		m_nowAnim.Draw(Vec2(m_pos.x - Game::kScreenWidth, m_pos.y), m_isTurn); // ��ʂ̍����ɂ��`��
	}

#ifdef _DEBUG
	// �����蔻��̕\��
	DrawBox(m_pos.x - (kHitBoxWidth / 2),m_pos.y - (kHitBoxHeight / 2), m_pos.x + (kHitBoxWidth / 2), m_pos.y + (kHitBoxHeight / 2),GetColor(255,0,0),false);
	//DrawFormatString(m_pos.x, m_pos.y + 50, GetColor(255, 255, 255), "m_deathTime:%d",m_deathTime);
#endif // _DEBUG
}

// �v���C���[�̏�Ԃ��X�V���āA���݂̃A�j���[�V������ݒ肷��
PlayerState Player::UpdateState()
{
	PlayerState ans;

	if (m_isAlive)
	{
		if (m_speed.y < -1)
		{
			if (m_state != PlayerState::Jump)
			{
				m_nowAnim = m_jumpAnim;
			}
			ans = PlayerState::Jump;
		}
		else if (m_speed.y > 1)
		{
			if (m_state != PlayerState::Fall)
			{
				m_nowAnim = m_fallAnim;
			}
			ans = PlayerState::Fall;
		}
		else if (m_speed.x > 1 || m_speed.x < -1)
		{
			if (m_state != PlayerState::Run)
			{
				m_nowAnim = m_runAnim;
			}
			ans = PlayerState::Run;
		}
		else
		{
			if (m_state != PlayerState::Idle)
			{
				m_nowAnim = m_idleAnim;
			}
			ans = PlayerState::Idle;
		}
	}
	else
	{
		if (m_state != PlayerState::Death)
		{
			PlaySoundMem(m_deathSE, DX_PLAYTYPE_BACK);
			m_nowAnim = m_deathAnim;
		}
		ans = PlayerState::Death;
	}

	return ans;
}

void Player::MapCollision(const Map& map,std::vector<Snail*>& pSnails)
{
	Vec2 playerSize = Vec2(kHitBoxWidth, kHitBoxHeight);
	Vec2 hitBlockPos;

	if (map.CheckCollision(m_pos, playerSize, hitBlockPos))
	{
		// �����蔻�肪�������ꍇ�̏���
		Vec2 dist;
		dist.x = m_pos.x - hitBlockPos.x;
		dist.y = m_pos.y - hitBlockPos.y;

		// X������Y�����̂ǂ���̋������傫�����Ŕ���
		if (abs(dist.x) > abs(dist.y))
		{
			// �����瓖�������ꍇ�i���E�̕ǁj
			if (dist.x > 0)
			{
				// �v���C���[���u���b�N�̉E���ɂ���i���̕ǂɓ��������j
				m_pos = (Vec2(hitBlockPos.x + kChipSize / 2 + playerSize.x / 2, m_pos.y));
			}
			else
			{
				// �v���C���[���u���b�N�̍����ɂ���i�E�̕ǂɓ��������j
				m_pos = (Vec2(hitBlockPos.x - kChipSize / 2 - playerSize.x / 2, m_pos.y));
			}
			// X�����̑��x��0�ɂ���
			m_speed = (Vec2(0, m_speed.y));
		}
		else
		{
			// �c���瓖�������ꍇ�i�㉺�j
			if (dist.y > 0)
			{
				// �v���C���[���u���b�N�̉����ɂ���i�ォ�瓖�������j
				if (m_speed.y < 0)	// �W�����v���Ȃ�(y�̑��x�����x���}�C�i�X�Ȃ�)
				{
					for (auto& snail : pSnails)
					{
						if (snail != nullptr)
						{
							CheckAttackSnail(*snail);	// �X�l�C���ɍU���������������`�F�b�N
						}
					}
					m_pos = (Vec2(m_pos.x, hitBlockPos.y + kChipSize / 2 + playerSize.y / 2));
					if (m_speed.y < -5)
					{
						m_speed.y = -5;
					}
				}
			}
			else
			{
				// �v���C���[���u���b�N�̏㑤�ɂ���i�����瓖�������E���n�j
				if (m_speed.y > 0)	// �������Ȃ�(y�̑��x���v���X�Ȃ�)
				{
					m_pos = (Vec2(m_pos.x, hitBlockPos.y - kChipSize / 2 - playerSize.y / 2));
					m_speed = (Vec2(m_speed.x, 0));
					m_isOnFloor = true;  // �n�ʂɐڒn
				}
			}
		}
	}
}

void Player::CheckAttackSnail(Snail& snail)
{
	float distY = abs(m_pos.y - snail.GetPos().y);
	float distX = abs(m_pos.x - snail.GetPos().x);
	bool isAttackDistY = (distY < kAttackHitBoxHeight && distY > kAttackHitBoxHeight - kSnailHitBoxHeight);	// �U�������Y�����̋���
	if (isAttackDistY && (distX < kAttackHitBoxWidth))
	{
		// �X�l�C���ɍU������������
		PlaySoundMem(m_snailAttackedSE, DX_PLAYTYPE_BACK);
		snail.Attacked();	// �X�l�C�����U������
	}
}

void Player::CheckHitSnail(Snail& snail)
{
	float distY = abs(m_pos.y - snail.GetPos().y);
	float distX = abs(m_pos.x - snail.GetPos().x);
	float hitBoxWidth = kHitBoxWidth / 2 + kSnailHitBoxWidth / 2;	// �v���C���[�̓����蔻��ƃX�l�C���̓����蔻��̕������킹��
	float hitBoxHeight = kHitBoxHeight / 2 + kSnailHitBoxHeight / 2;	// �v���C���[�̓����蔻��ƃX�l�C���̓����蔻��̍��������킹��
	bool isHit = (distY < hitBoxHeight && distX < hitBoxWidth);
	if (isHit && snail.GetIsAlive())
	{
		// �X�l�C���ɐG�ꂽ
		//printfDx("�X�l�C���ɐG�ꂽ\n");
		bool isFliped = snail.GetIsFliped();
		if (isFliped)
		{
			// �X�l�C�����Ђ�����Ԃ��Ă���ꍇ�͓|��
			PlaySoundMem(m_snailDefeatedSE, DX_PLAYTYPE_BACK);
			snail.Dead();	// �X�l�C����|��
		}
		else
		{
			// �v���C���[���|�����
			if (!m_isInvincible)	// ���G��ԂłȂ����
			{
				m_isAlive = false;
			}
		}
	}

}
