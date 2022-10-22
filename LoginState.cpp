//#include "LoginState.h"
//#include "LoginManager.h"
//#include "LogManager.h"
//void LoginState::OnRecv()
//{
//	// 로그인 상태에서의 Recv 프로세스
//	LoginManager::GetInstance()->Function(m_session);
//}
//
//void LoginState::OnSend()
//{
//	// 로그인상태에서 다음 상태로 변경될때 조건과 작업을 넣기...
//	if (m_isLogin)
//	{
//		// 만들 예정
//		//m_session->ChangeState(m_session->GetSomtingState());
//	}
//}
//
//bool LoginState::GetIsLogin()
//{
//	return m_isLogin;
//}
//
//void LoginState::SetIsLogin(bool _value)
//{
//	m_isLogin = _value;
//}
//
//LoginState::LoginState(Session* _session) : State(_session)
//{
//	m_isLogin = false;
//}
