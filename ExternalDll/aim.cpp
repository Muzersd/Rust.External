#include "aim.cpp"

void real_entry()
{
	OBJECT_ATTRIBUTES obj_att = { 0 };
	HANDLE thread = NULL;
	DbgPrintEx(0, 0, "asdas.");

	clean_piddbcachetalbe();

	InitializeObjectAttributes(&obj_att, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	 NTSTATUS status = PsCreateSystemThread(&thread, THREAD_ALL_ACCESS, &obj_att, NULL, NULL, create_memeory_thread, NULL);
	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(0, 0, "sad asdsad:\n", status);
		return status;
	}

	HkDetourFunction(get_system_module_export("\\SystemRoot\\System32\\drivers\\watchdog.sys", "WdLogEvent5_WdError"), (PVOID)hooked_event, 16, (PVOID*)&original_event);

	DbgPrintEx(0, 0, "sad sa d!");
	ZwClose(thread);
}




bool InFov(class BasePlayer& BasePlayer_on_Aimming, enum BoneList bone)
{

	Vector2 ScreenPos;
	if (!myLocalPlayer.WorldToScreen(BasePlayer_on_Aimming.GetBonePosition(head), &ScreenPos)) return false;

	return Math::Calc2D_Dist(Vector2(Vars::Config::ScreenWidth / 2, Vars::Config::ScreenHigh / 2), ScreenPos) <= Vars::Aim::fov;
	
	if (pTarget) {
		m_TargetExist = true;
		m_TargetData.pCoreObject = pTarget->pCoreObject;
		m_TargetData.pGameObject = pTarget->pGameObject;
		m_TargetData.pVisuaState = pTarget->pVisuaState;
		m_TargetData.pOwnClassObject = pTarget->pOwnClassObject;
		return true;
	}

	return false;
}



void Normalize(Vector2& angle)
{
	if (angle.x > 89)angle.x = 89;
	if (angle.x < -89)angle.x = -89;
	while (angle.y > 180)angle.y -= 360;
	while (angle.y < -180)angle.y += 360;
}


float GetBulletSpeed()
{
	switch (myLocalPlayer.myActiveWeapon.GetID())
	{
	case 1545779598: //ak47
		return 375.f;
	case 2482412119: //lr300
		return 375.f;
	case 3390104151: //semi-rifle
		return 375.f;
	case 28201841: //m39
		return 375.f* 1.16f;
	case 2225388408: //m249
		return 375.f * 1.4f;

	case 1588298435: //bolt
		return 375.f * 1.8f;
	case 3516600001: //l96	
		return 375.f * 3.2f;

	case 1318558775: //mp5a4
		return 300.f * 0.8f;
	case 1796682209: //smg
		return 300.f * 0.8f;
	case 2536594571: //thompson
		return 300.f;
	case 3442404277: //m92
		return 300.f;
	case 818877484: //semi-pistol
		return 300.f;
	case 1373971859: //python
		return 300.f;
	case 649912614: //revolver
		return 300.f;

	default:
		return 0.f;

		/*
	case 1801741824: //bolt
		return 656.25f;
	case 1798229440: //l96
		return 1125.f;
	case 1802481984: //m39
		return 469.f;
	case 1798228096: //revolver
		return 300.f;
	case 818877484: //p250
		return 300.f;
	case 1802077840: //waterpipe, 250-green ammo, 100-other
		return 100.f;
	case 1798227968: //double barrel, 250-green ammo, 100-other
		return 100.f;
		*/


	}
}


//unuse
double CalcBulletDrop(double height, double DepthPlayerTarget, float velocity, float gravity) {
	double pitch = (atan2(height, DepthPlayerTarget));
	double BulletVelocityXY = velocity * cos(pitch);
	double Time = DepthPlayerTarget / BulletVelocityXY;
	double TotalVerticalDrop = (0.5f * gravity * Time * Time);
	return TotalVerticalDrop * 10;
}

Vector3 Prediction(const Vector3& my_Pos, BasePlayer& BasePlayer_on_Aimming, BoneList Bone)
{


	Vector3 BonePos = BasePlayer_on_Aimming.GetBonePosition(Bone);

	float Dist = Math::Calc3D_Dist(my_Pos, BonePos);

	float BulletSpeed= GetBulletSpeed();
	if (BulletSpeed <= 0)return BonePos;

	float BulletTime = Dist / BulletSpeed;
	Vector3 vel = BasePlayer_on_Aimming.GetVelocity();
	Vector3 Predict = vel * BulletTime;
	BonePos += Predict;


	float DegAngle = myLocalPlayer.GetBA().x;
	float coef = 1;
	if (DegAngle >= 10 && Dist <=100)
	{
		coef = cos(DEG2RAD(DegAngle)) * 0.1f;
	}
	else if(DegAngle>=0)
	{
		coef = cos(DEG2RAD(DegAngle)) * 0.9f;
	}
	BonePos.y += (6.4f * BulletTime * BulletTime) * coef;//4.94f


	//std::cout << "coef: "<< coef << std::endl;
	//std::cout << "DegAngle: " << DegAngle << std::endl;
	//BonePos.y -= BonePos.y*1.f / Dist;

	return  BonePos;

static Aimbot::Aimbot_Data Aimbot_Data;
{
	Process(Valorant.exe, "Valorant.exe");
	
	smooth = Aimbot_Data.smooth;
	fov = Aimbot_Data.fov;
	bone = Aimbot_Data.bone;
	aim_key = Aimbot_Data.aim_key;
	aim_key_pressed = Aimbot_Data.aim_key_pressed;
	aim_key_pressed_time = Aimbot_Data.aim_key_pressed_time;
	aim_key_pressed_time_max = Aimbot_Data.aim_key_pressed_time_max;
	aim_key_pressed_time_min = Aimbot_Data.aim_key_pressed_time_min;

{
	shutdown_time = Aimbot_Data.shutdown_time;
	shutdown_time_max = Aimbot_Data.shutdown_time_max;
	#ifdef _DEBUG
		std::cout << "shutdown_time: " << shutdown_time << std::endl;
		std::cout << "shutdown_time_max: " << shutdown_time_max << std::endl;
		
}
}	
	
}

void GoToTarget(BasePlayer &BasePlayer_on_Aimming, BoneList bone)
{


	Vector3 Local = myLocalPlayer.GetBonePosition(head);
	Vector3 PlayerPos = Prediction(Local, BasePlayer_on_Aimming, bone);
	Vector2 Offset = Math::CalcAngle(Local, PlayerPos) - myLocalPlayer.GetBA();
	//std::cout << "Local:" << Local.x << " " << Local.y <<" "<< Local.z<< std::endl;


	Normalize(Offset);

	//std::cout << "Offset:" << Offset.x << " " << Offset.y << std::endl;
	Offset.x *= 1.0f - (Vars::Aim::smooth* 0.3+0.4);
    Offset.y *= 1.0f - (Vars::Aim::smooth *0.3+0.4);

	Vector2 AngleToAim = myLocalPlayer.GetBA() + Offset;
	myLocalPlayer.SetBA(AngleToAim);
}

void Aim(DWORD64& BasePlayer_on_Aimming)
{
	static BasePlayer Player;
	Player.set_addr(BasePlayer_on_Aimming);


	if (BasePlayer_on_Aimming && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{

		if (Player.IsDead())BasePlayer_on_Aimming = NULL;
		else
		{

			static int boneArr[6] = { head ,spine1 ,r_upperarm ,l_breast ,r_hand };
			static BoneList bone;
			static DWORD64 isBasePlayerChange = NULL;
			if (isBasePlayerChange != Player.get_addr())
			{
				if (Vars::Aim::randomBone)bone = BoneList(boneArr[int(rand() % 6)]);
				else bone = head;

				isBasePlayerChange = Player.get_addr();
			}

			static int start = 0;
			static int summ = 0;

			summ += clock() - start;
			start = clock();

			if (summ >= 20)
			{
				GoToTarget(Player, bone);
				summ = 0;
			}



			
			//óâåëèâèâàåì smooth-óõóäøàåì êîíòðîëü îòäà÷è
			static int RCSstart = 0;
			static int RCSsumm = 0;

			RCSsumm += clock() - RCSstart;
			RCSstart = clock();
		}


	}
	else BasePlayer_on_Aimming=NULL;
}

void Rust::Aimbot::exec()
{
	bool buttonPressed = (GetAsyncKeyState(VK_XBUTTON2)) && 0x8000; //VK_XBUTTON1 -> mouse back button

	if (buttonPressed) {
		if(!m_TargetExist)
			if (!FindTarget())
				return;

		if (!Rust::CheatStruct::Player::isAlive(m_TargetData.pOwnClassObject)) {
			m_TargetExist = false;
			return;
		}

		auto TargetHeadPos = Rust::MainCam::GetPosition(Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(m_TargetData.pOwnClassObject, {0x80, 0x28, 0x10 }));
		auto LocalHeadPos = Rust::MainCam::GetPosition(Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(Rust::Globals::hack_data.LocalPlayer.pOwnClassObject, { 0x80, 0x28, 0x10 }));
		auto AngleAddress = Rust::Globals::hack_data.RustMemory->Read<uint64_t>(Rust::Globals::hack_data.LocalPlayer.pOwnClassObject + 0x470);
		auto OriginalAngle = Rust::Globals::hack_data.RustMemory->Read<Cheat::Vector2>(AngleAddress + 0x44);

		if (Rust::Globals::hack_setting.Aimbot.prediction)
			Apply_Predicition(TargetHeadPos);

		auto Offset = CalcAngle(LocalHeadPos, TargetHeadPos) - OriginalAngle;
		SmoothAim(Offset, 2);
		Normalize(Offset.y, Offset.x);

		auto AngleToAim = OriginalAngle + Offset;
		Rust::Globals::hack_data.RustMemory->Write(AngleToAim, AngleAddress + 0x44);
	}
	else {
		m_TargetExist = false;
	}
}

	void Update() {
		while (true) {

			if (Globals::tWnd == GetForegroundWindow()) {

			}

			if (GetAsyncKeyState(0x2D)) {
				if (!Globals::bShowMenu) {
					long winlong = GetWindowLong(Globals::hWnd, GWL_EXSTYLE);

					if (winlong != WS_EX_LAYERED | WS_EX_TOPMOST)
						SetWindowLong(Globals::hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}

				if (Globals::bShowMenu) {
					long winlong = GetWindowLong(Globals::hWnd, GWL_EXSTYLE);

					if (winlong != WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT)
						SetWindowLong(Globals::hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
				}
				Globals::bShowMenu = !Globals::bShowMenu;

				while (GetAsyncKeyState(0x2D)) {}
			}

			//CHANGEDTIME
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
		}
	}
}
