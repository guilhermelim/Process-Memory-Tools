//#define _USE_MATH_DEFINES
//#define D_USE_MATH_DEFINES
//#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "memory.h"
#include "fly_mode.h"
#include "Converter.h"

void updatePosition(DWORD processId, uintptr_t xAddress, uintptr_t yAddress, uintptr_t zAddress, uintptr_t camXAddress, uintptr_t camYAddress) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	float speed = 1.4f;
	
	// Read and convert camera values
	float camX = readProcess<float>(hProcess, camXAddress);
	float camY = readProcess<float>(hProcess, camYAddress);
	float camXDegrees = Converter::radiansToDegrees(camX);
	float camYDegrees = Converter::scaleToDegrees(camY, -1.5f, 1.5f, -90.0f, 90.0f);

	auto readAndValidate = [&](uintptr_t address) -> std::optional<float> {
		float value = readProcess<float>(hProcess, address);
		return std::isnan(value) ? std::optional<float>{} : value;
	};

	// Check if keys are pressed
    bool isWPressed = (GetAsyncKeyState('W') & 0x8000) != 0;
    bool isSPressed = (GetAsyncKeyState('S') & 0x8000) != 0;
    bool isAPressed = (GetAsyncKeyState('A') & 0x8000) != 0;
    bool isDPressed = (GetAsyncKeyState('D') & 0x8000) != 0;
    bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    bool isSpacePressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if (isWPressed) {
        //std::cout << "W is pressed" << std::endl;
		auto xValue = readAndValidate(xAddress);
		auto yValue = readAndValidate(yAddress);
		auto zValue = readAndValidate(zAddress);
		if (xValue && yValue && zValue) {
			Coordinates<float> newCoords = flyMode(*xValue, *yValue, *zValue, camXDegrees, camYDegrees, speed, FORWARD);			
			writeProcess(hProcess, xAddress, newCoords.x);
			writeProcess(hProcess, yAddress, newCoords.y);
			writeProcess(hProcess, zAddress, newCoords.z);
		}
    }
    if (isSPressed) {
		//std::cout << "S is pressed" << std::endl;
		auto xValue = readAndValidate(xAddress);
		auto yValue = readAndValidate(yAddress);
		auto zValue = readAndValidate(zAddress);
		if (xValue && yValue && zValue) {
			Coordinates newCoords = flyMode(*xValue, *yValue, *zValue, camXDegrees, camYDegrees, speed, BACKWARD);
			writeProcess(hProcess, xAddress, newCoords.x);
			writeProcess(hProcess, yAddress, newCoords.y);
			writeProcess(hProcess, zAddress, newCoords.z);
		}
    }
	if (isAPressed) {
		//std::cout << "A is pressed" << std::endl;
		auto xValue = readAndValidate(xAddress);
		auto yValue = readAndValidate(yAddress);
		auto zValue = readAndValidate(zAddress);
		if (xValue && yValue && zValue) {
			Coordinates newCoords = flyMode(*xValue, *yValue, *zValue, camXDegrees, camYDegrees, speed, LEFT);
			writeProcess(hProcess, xAddress, newCoords.x);
			writeProcess(hProcess, yAddress, newCoords.y);
			writeProcess(hProcess, zAddress, newCoords.z);
		}
	}
    if (isDPressed) {
		//std::cout << "D is pressed" << std::endl;
		auto xValue = readAndValidate(xAddress);
		auto yValue = readAndValidate(yAddress);
		auto zValue = readAndValidate(zAddress);
		if (xValue && yValue && zValue) {
			Coordinates newCoords = flyMode(*xValue, *yValue, *zValue, camXDegrees, camYDegrees, speed, RIGHT);
			writeProcess(hProcess, xAddress, newCoords.x);
			writeProcess(hProcess, yAddress, newCoords.y);
			writeProcess(hProcess, zAddress, newCoords.z);
		}
    }
    if (isShiftPressed) {
		//std::cout << "Shift is pressed" << std::endl;
		auto yValue = readAndValidate(yAddress);
		if (yValue) {
			writeProcess(hProcess, yAddress, *yValue - speed);
		}
    }
    if (isSpacePressed) {
		//std::cout << "Space is pressed" << std::endl;
		auto yValue = readAndValidate(yAddress);
		if (yValue) {
			writeProcess(hProcess, yAddress, *yValue + (speed));
		}
    }       
	
	CloseHandle(hProcess);
}

int main() {	
	std::string processName = "thedivision.exe";
	HANDLE hProcess = getProcessHandleByName(processName);
	DWORD idProcess = getProcessId(processName);

	if (hProcess != nullptr) {
		std::cout << "Handle for process " << processName << ": " << hProcess << std::endl;
		CloseHandle(hProcess);
	}
	else {
		std::cout << "Failed to obtain handle for process " << processName << std::endl;
	}

	if (idProcess != 0) {
		std::cout << "Id for process " << processName << ": " << idProcess << std::endl;

	}
	else {
		std::cout << "Failed to obtain id for process " << processName << std::endl;
	}

	std::vector<uintptr_t> offsetsPlayerPosition = { 0x228, 0x50, 0x1D0, 0x220, 0x278 };
	uintptr_t playerPositionAddress = getPointerAddress(idProcess, "TheDivision.exe", 0x04508B80, offsetsPlayerPosition);
	
	std::vector<uintptr_t> offsetsCamera = { 0x10, 0x2D0, 0x18, 0xB0, 0x78 };
	uintptr_t cameraAddress = getPointerAddress(idProcess, "TheDivision.exe", 0x0477FCF0, offsetsCamera);

	uintptr_t xAddress = getPointerAddress(idProcess, playerPositionAddress, { 0x70 });
	uintptr_t yAddress = getPointerAddress(idProcess, playerPositionAddress, { 0x74 });
	uintptr_t zAddress = getPointerAddress(idProcess, playerPositionAddress, { 0x78 });

	uintptr_t camXAddress = getPointerAddress(idProcess, cameraAddress, { 0xC0 });
	uintptr_t camYAddress = getPointerAddress(idProcess, cameraAddress, { 0xC4 });

	uintptr_t moveAssemblyAddress = getProcessModuleByName(idProcess, "TheDivision.exe").baseAddress + 0x22D5FC3;

	std::cout << "Address xAddress calculated: " << std::hex << std::uppercase << xAddress << std::endl;
	std::cout << "Address camXAddress calculated: " << std::hex << std::uppercase << camXAddress << std::endl;
	std::cout << "Address moveAssemblyAddress calculated: " << std::hex << std::uppercase << moveAssemblyAddress << std::endl;

	int timerInterval = 100;

	bool flyModeActive = false;
	bool prevKeyState = false;

	while (true) {		
		bool currentKeyState = (GetAsyncKeyState('Y') & 0x8000) != 0;

		if (currentKeyState && !prevKeyState) { // Detect key press event
			flyModeActive = !flyModeActive; // Toggle fly mode
			std::cout << "Fly Mode " << (flyModeActive ? "Activated" : "Deactivated") << std::endl;

			/*if (flyModeActive) {
				BYTE nop[] = { 0x90, 0x90, 0x90, 0x90 };
				writeProcess(idProcess, moveAssemblyAddress, nop);
			}
			else {
				BYTE moveAssemblyHex[] = { 0x41, 0x0F, 0x29, 0x06 };
				writeProcess(idProcess, moveAssemblyAddress, moveAssemblyHex);	
			}*/
		}

		prevKeyState = currentKeyState; // Update previous key state

		if (flyModeActive) {			
			updatePosition(idProcess, xAddress, yAddress, zAddress, camXAddress, camYAddress);
		}	

		std::this_thread::sleep_for(std::chrono::milliseconds(timerInterval));
		//system("cls");
	}

	return 0;
}
