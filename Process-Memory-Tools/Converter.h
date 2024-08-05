#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>
#include <string>
#include <Windows.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Converter {
public:
    // Função estática que aceita tanto CHAR* quanto WCHAR* e converte para std::string
    template<typename T>
    static std::string toString(const T* str) {
        return convert(str);
    }

    static float radiansToDegrees(float radians) {
        return radians * (180.0f / static_cast<float>(M_PI));
    }

    static float degreesToRadians(float degrees) {
        return degrees * (static_cast<float>(M_PI) / 180.0f);
    }

    static float scaleToDegrees(float scaleValue, float maxDegrees, float scaleMax) {
        // Calcula o fator de escala baseado na escala máxima fornecida
        float scaleFactor = maxDegrees / scaleMax;
        return scaleValue * scaleFactor;
    }

    static float scaleToDegrees(float scaleValue, float scaleMin, float scaleMax, float degreesMin, float degreesMax) {
        // Calcula o intervalo da escala
        float scaleRange = scaleMax - scaleMin;
        // Calcula o intervalo em graus
        float degreesRange = degreesMax - degreesMin;
        // Calcula o fator de conversão
        float scaleFactor = degreesRange / scaleRange;
        // Converte o valor da escala para graus
        return (scaleValue - scaleMin) * scaleFactor + degreesMin;
    }

private:
    // Função privada estática para converter WCHAR para std::string
    static std::string convert(const WCHAR* wcharStr) {
        if (wcharStr == nullptr)
            return "";

        // Calcula o tamanho necessário para a string de destino
        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, NULL, 0, NULL, NULL);
        if (sizeNeeded == 0)
            return "";

        // Aloca espaço para a string de destino
        std::string str(sizeNeeded - 1, 0); // -1 para remover o caractere nulo adicional

        // Realiza a conversão
        WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, &str[0], sizeNeeded, NULL, NULL);

        return str;
    }

    // Função privada estática para converter CHAR para std::string
    static std::string convert(const CHAR* charStr) {
        if (charStr == nullptr)
            return "";

        return std::string(charStr);
    }
};

#endif
