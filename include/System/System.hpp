#pragma once

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <windows.h>

#include "Input/Input.hpp"
#include "Application/Application.hpp"

class System
{
public:
	System();
	System(const System&);
	~System();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	std::unique_ptr<Input> m_input;
	std::unique_ptr<Application> m_application;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System* applicationHandle = 0;