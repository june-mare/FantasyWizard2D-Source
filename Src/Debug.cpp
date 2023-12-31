#include "Debug.h"
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

namespace Debug {
	/*書式付き文字列をデバッグウィンドウに表示する*/

	void Log(Type type, const char* func, const char* format, ...) {
		char buffer[1024];
	
		char* p = buffer;
		size_t size = sizeof(buffer) - 1;//末尾に\nを追加するため
		//ログの種類と関数名を設定
		static const char* const typeNames[]{ "エラー","警告","情報" };
		size_t n = snprintf(p,size, "[%s]%s:", typeNames[static_cast<int>(type)], func);

		p += n;
		size -= n;

		//メッセージを設定
		va_list ap;
		va_start(ap, format);

		p += vsnprintf(p, size, format, ap);
		va_end(ap);

		//末尾に改行を追加
		p[0] = '\n';
		p[1] = '\0';

		//作成した文字列をデバッグウィンドウに表示
		OutputDebugString(buffer);
	}

}