#pragma once
#ifndef DEBUG_H_INCLUDE
#define DEBUG_H_INCLUDE
namespace Debug {
	/*���b�Z�[�W�̎��*/
	enum class Type {
		error,		//�G���[
		warning,	//�x��
		info,		//���̑��̏��
	};

	void Log(Type type, const char* func, const char* format, ...);

}

# define ENABLE_DEBUG_LOG	//���̃}�N�����`����ƃf�o�b�O�o�͂��L��
#ifdef NDEBUG
#endif //NDEBUG

//�f�o�b�O�o�̓}�N��
#ifdef ENABLE_DEBUG_LOG
#define LOG_ERROR(...)	Debug::Log(Debug::Type::error,__func__,__VA_ARGS__) 
#define LOG_WARNING(...)	Debug::Log(Debug::Type::warning,__func__,__VA_ARGS__) 
#define LOG(...)	Debug::Log(Debug::Type::info,__func__,__VA_ARGS__) 

#else
#define LOG_ERROR(...)  ((void)0)	
#define LOG_WARNING(...)((void)0)	
#define LOG(...)		((void)0)	

#endif // ENABLE_DEBUG_LOG

#endif // !DEBUG_H_INCLUDE