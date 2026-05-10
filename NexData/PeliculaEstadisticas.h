#pragma once

template <typename T_Format>
class PeliculaStats {
public:
    T_Format Puntuacion;
    int Volumen;
    int VistasTotales;

    PeliculaStats(T_Format punt = 0, int vol = 0, int visits = 0)
        : Puntuacion(punt), Volumen(vol), VistasTotales(visits) {
    }
};