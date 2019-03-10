#include "CrazyStormWin32Interop.h"

static long _send_command(CrazyStormInstance * instance, const char * command) {
	long written;
	WriteFile(instance->StdInWrite, command, strlen(command), &written, NULL);
	return written;
}

inline void _set_renderable(Renderable * renderable, float x, float y, float w, float h, long t) {
	renderable->Center = vector2_create(x, y);
	renderable->Scale = vector2_create(w, h);
	renderable->Color = vector4_create(1.0f, 1.0f, 1.0f, 1.0f);
	renderable->TextureRegion.TextureID = resources.Images.Barrages;
	renderable->TextureRegion.Rect = get_projectile_type_descritor(t + 1).Region;
}

static void _fetch_projs(CrazyStormInstance * instance) {
	_send_command(instance, "g\r\n");
	char size[11];
	long read;
	long avail = 0;
	long remain;
	while (avail < 10)
		PeekNamedPipe(instance->StdOutRead, NULL, 0, NULL, &avail, &remain), Sleep(1);
	ReadFile(instance->StdOutRead, size, 10, &read, NULL);
	size[10] = '\0';
	long rsize;
	sscanf(size, "%ld", &rsize);
	if (rsize == 0)
		return;
	char * buf = malloc(rsize);
	avail = 0;
	while (avail < rsize)
		PeekNamedPipe(instance->StdOutRead, NULL, 0, NULL, &avail, &remain), Sleep(1);
	ReadFile(instance->StdOutRead, buf, rsize, &read, NULL);
	long parsed = 0;
	long add = 0;
	long alpha = 0;
	while (parsed < rsize) {
		float x, y, w, h;
		long t;
		unsigned char b;
		x = *(float *)(buf + parsed);
		parsed += 4;
		y = *(float *)(buf + parsed);
		parsed += 4;
		t = *(long *)(buf + parsed);
		parsed += 4;
		w = *(float *)(buf + parsed);
		parsed += 4;
		h = *(float *)(buf + parsed);
		parsed += 4;
		b = *(unsigned char *)(buf + parsed);
		parsed += 1;
		if (b == 0) {
			if (alpha >= instance->_alphablendActive) {
				instance->_alphablend[alpha].Layer = RENDER_LAYER_ENTITY_7;
				register_renderable(&instance->_alphablend[alpha]);
				++instance->_alphablendActive;
			}
			_set_renderable(&instance->_alphablend[alpha], x, y, w, h, t);
			alpha++;
		}
		else {
			if (add >= instance->_additiveActive) {
				instance->_additive[add].Layer = RENDER_LAYER_ENTITY_6;
				register_renderable(&instance->_additive[add]);
				++instance->_additiveActive;
			}
			_set_renderable(&instance->_additive[add], x, y, w, h, t);
			add++;
		}
	}
	for (int i = add; i < instance->_additiveActive; i++) {
		unregister_renderable(&instance->_additive[i]);
	}
	instance->_additiveActive = add;
	for (int i = alpha; i < instance->_alphablendActive; i++) {
		unregister_renderable(&instance->_alphablend[i]);
	}
	instance->_alphablendActive = alpha;
	free(buf);
}

CrazyStormInstance * crazy_storm_start(const char * path2exe) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	CrazyStormInstance * retVal = calloc(1, sizeof(CrazyStormInstance));

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.dwFlags |= STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	CreatePipe(&retVal->StdOutRead, &retVal->StdOutWrite, &saAttr, 0);
	SetHandleInformation(retVal->StdOutRead, HANDLE_FLAG_INHERIT, 0);
	CreatePipe(&retVal->StdInRead, &retVal->StdInWrite, &saAttr, 0);
	SetHandleInformation(retVal->StdInWrite, HANDLE_FLAG_INHERIT, 0);

	si.hStdInput = retVal->StdInRead;
	si.hStdOutput = retVal->StdOutWrite;
	si.hStdError = retVal->StdOutWrite;

	CreateProcessA(path2exe, NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	retVal->ProcessInformation = pi;

	CloseHandle(retVal->StdInRead);
	CloseHandle(retVal->StdOutWrite);

	return retVal;
}

void crazy_storm_load_mbg(CrazyStormInstance * instance, const char * path2mbg) {
	char buf[512];
	sprintf(buf, "o%s\r\n", path2mbg);
	_send_command(instance, buf);
}

void crazy_storm_update(CrazyStormInstance * instance) {
	_send_command(instance, "u\r\n");
	_fetch_projs(instance);
}

void crazy_storm_terminate(CrazyStormInstance * instance) {
	_send_command(instance, "q\r\n");
	WaitForSingleObject(instance->ProcessInformation.hProcess, 1000);
	CloseHandle(instance->StdInWrite);
	CloseHandle(instance->StdOutRead);
	CloseHandle(instance->ProcessInformation.hProcess);
	CloseHandle(instance->ProcessInformation.hThread);
}
