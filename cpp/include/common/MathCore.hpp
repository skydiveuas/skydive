// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __ROBOLIB_CORE__
#define __ROBOLIB_CORE__

#define MAX_QR_ALGORITHM_ITERATIONS 1000 // maksymalna liczba iteracji algorytmu QR

#include <cmath>
#include <climits>

#ifdef __SKYDIVE_USE_STL__

#include <ostream>

#endif //__SKYDIVE_USE_STL__

namespace roboLib
{
const double pi = 3.1415926535897932;		// liczba pi
const double gravity = 9.8105;				// przyspieszenie ziemskie w Krakowie [m/s^2]
const double rhoAir = 1.168;				// gestosc powietrza w warukach normalnych [kg/m^3]
const double rEarth = 6371000;				// przyblizony promien Ziemi [m]
const double absoluteZeroCelsius = -273.15;	// temperatura zera bezwzglednego w skali Celsiusza
const double R = 29.2746;					// stala gazowa
const double tau = 6.5 / 1000.0;			// gradnient temperaturowy atmosfery [K/m]
const double normTemperature = 288.15;		// normal temperature for sea level [K]
const double normPressure = 1013.25;		// normal preassure for sea level [hPa]

template <typename _Tp> inline _Tp toRad(const _Tp deg); // przeksztalcenie kata ze stopni na radiany
template <typename _Tp> inline _Tp toDeg(const _Tp rad); // przeksztalcenie kata z radianow na stopnie

template <typename _Tp> inline _Tp toKelvin(const _Tp celsius); // przeksztalcenie stopni Celsiusza na Kelvina
template <typename _Tp> inline _Tp toCelcius(const _Tp kelvin); // przeksztalcenie stopni Kelvin na Celsiusza

template <typename _Tp> inline _Tp minmaxVal(const _Tp min, const _Tp max, const _Tp val); // ustalenie wartosci w przedziale [min,max]
template <typename _Tp> inline _Tp minmaxVal(const _Tp minmaxAbs, const _Tp val);

template <typename _Tp> unsigned short valToShort(const _Tp min, const _Tp max, const _Tp val); // konwersja z floata do unsigned shorta na zadanym zakresie [min,max]
template <typename _Tp> _Tp shortToVal(const _Tp min, const _Tp max, const unsigned short val); // konwersja z unsigned shortaf do loata na zadanym zakresie [min,max]
}


// ================== ZDEFINIOWANE KLASY ==================
template <class _Tp> class Vect2D; // wektor 2-elementowy (x,y)
template <class _Tp> class Vect3D; // wektor 3-elemntowy (x,y,z)
template <class _Tp> class Vect4D; // wektor 4-elementowy (kwaternion)
template <class _Tp> class Mat2D; // macierz 2x2 
template <class _Tp> class Mat3D; // macierz 3x3 (rotacji 3D)
template <class _Tp> class Mat4D; // macierz 4x4 

template <class _Tp> class Vector; // dynamicznie alokowany wektor dowolnego rozmiaru
template <class _Tp> class Matrix; // dynamicznie alokowana macierz kwadratowa dowolnego rozmiaru
// ========================================================

// konwencja katow RPY !!!
// roll (x), pitch (y), yaw (z) - 321


// ==================== PODSTAWOWE TYPY ===================
// Vect2D
typedef Vect2D <short> Vect2Ds;
typedef Vect2D <float> Vect2Df;
typedef Vect2D <double> Vect2Dd;
typedef Vect2D <long double> Vect2Ddd;
// Vect3D
typedef Vect3D <short> Vect3Ds;
typedef Vect3D <float> Vect3Df;
typedef Vect3D <double> Vect3Dd;
typedef Vect3D <long double> Vect3Ddd;
// Vect4D
typedef Vect4D <short> Vect4Ds;
typedef Vect4D <float> Vect4Df;
typedef Vect4D <double> Vect4Dd;
typedef Vect4D <long double> Vect4Ddd;
// Mat2D
typedef Mat2D <float> Mat2Df;
typedef Mat2D <double> Mat2Dd;
typedef Mat2D <long double> Mat2Ddd;
// Mat3D
typedef Mat3D <float> Mat3Df;
typedef Mat3D <double> Mat3Dd;
typedef Mat3D <long double> Mat3Ddd;
// Mat4D
typedef Mat4D <float> Mat4Df;
typedef Mat4D <double> Mat4Dd;
typedef Mat4D <long double> Mat4Ddd;

// Vector
typedef Vector <short> Vectors;
typedef Vector <float> Vectorf;
typedef Vector <double> Vectord;
typedef Vector <long double> Vectordd;
// Matrix
typedef Matrix <float> Matrixf;
typedef Matrix <double> Matrixd;
typedef Matrix <long double> Matrixdd;
// ========================================================


template <class _Tp> class Vect2D // wektor 2-elemntowy (x,y)
{
public:
    // wsporzedne
    // zastosowanie: wspolrzedne kartezjanskie [x; y], wspolrzedne geograficzne [lat; lon]
    _Tp x, y;

    // konstruktory
    inline Vect2D(void);
    inline Vect2D(_Tp _x, _Tp _y);
    explicit Vect2D(const _Tp* vectTab);
    template <typename _Type> inline Vect2D(const Vect2D <_Type> &v);

    Vect2D(const Vector<_Tp>& vect);

    // operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
    inline _Tp& operator () (const unsigned row);
    inline const _Tp& operator () (const unsigned row) const; // tylko odczyt

    inline unsigned size() const; // rozmiar wektora

    // operatory logiczne
    template <typename _Type> inline bool operator == (const Vect2D <_Type> &v) const; // rownosc
    template <typename _Type> inline bool operator != (const Vect2D <_Type> &v) const; // nierownosc


    // operatory arytmetyczne
    template <typename _Type> inline Vect2D <_Tp> operator + (const _Type a) const; // suma
    template <typename _Type> inline Vect2D <_Tp> operator + (const Vect2D <_Type> &v) const;

    template <typename _Type> inline Vect2D <_Tp> operator - (const _Type a) const; // rozniaca
    template <typename _Type> inline Vect2D <_Tp> operator - (const Vect2D <_Type> &v) const;

    template <typename _Type> inline Vect2D <_Tp> operator * (const _Type a) const; // monozenie
    template <typename _Type> inline Vect2D <_Tp> operator * (const Vect2D <_Type> &v) const;

    template <typename _Type> inline Vect2D <_Tp> operator / (const _Type a) const; // dzielenie
    template <typename _Type> inline Vect2D <_Tp> operator / (const Vect2D <_Type> &v) const;


    // operacje analityczne
    template <typename _Type> _Tp getDistance(const Vect2D <_Type> &v) const; // wyznaczenie odleglosci miedzy koncami wektorow w ukladzie kartezjanskim
    template <typename _Type> _Tp getGeoDistance(const Vect2D <_Type> &geoPoint) const; // wyznaczenie odleglosci miedzy koncami wektorow w ukladzie wspolrzednych geograficznych (po sferze ziemskiej)

    inline _Tp getNorm(void) const; // obliczanie normalnej wektora
    Vect2D <_Tp> getNormal(void) const; // obliczanie wektora normalnego
    Vect2D <_Tp> getSqrt(void) const; // wyznaczenie pierwiastka kwadratowego wektora

    template <typename _Type> Vect2D <_Tp> toCartesian(const Vect2D<_Type>& origin) const;
    template <typename _Type> Vect2D <_Tp> toGeographic(const Vect2D<_Type>& origin) const;

#ifdef __SKYDIVE_USE_STL__

    template <typename _Type>
    friend std::ostream& operator << (std::ostream& stream, const Vect2D<_Type>& vect);

#endif //__SKYDIVE_USE_STL__
};

template <class _Tp> class Vect3D // wektor 3-elemntowy (x,y,z)
{
public:
    // wsporzedne
    _Tp x, y, z;

    // konstruktory
    inline Vect3D(void);
    inline Vect3D(_Tp _x, _Tp _y, _Tp _z);
    explicit Vect3D(const _Tp* vectTab);
    template <typename _Type> inline Vect3D(const Vect3D <_Type> &v);
    template <typename _Type> inline Vect3D(const Vect2D <_Tp> &v, const _Type z);

    Vect3D(const Vector<_Tp>& vect);

    // operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
    inline _Tp& operator () (const unsigned row);
    inline const _Tp& operator () (const unsigned row) const; // tylko odczyt

    Vect2D<_Tp> getVect2D(void) const; // wektora 2D z wspolrzednych x i y wektora 3D

    inline unsigned size() const; // rozmiar wektora


    // operatory logiczne
    template <typename _Type> inline bool operator == (const Vect3D <_Type> &v) const; // rownosc
    template <typename _Type> inline bool operator != (const Vect3D <_Type> &v) const; // nierownosc


    // operatory arytmetyczne
    template <typename _Type> inline Vect3D <_Tp> operator + (const _Type a) const; // suma
    template <typename _Type> inline Vect3D <_Tp> operator + (const Vect3D <_Type> &v) const;

    template <typename _Type> inline Vect3D <_Tp> operator - (const _Type a) const; // rozniaca
    template <typename _Type> inline Vect3D <_Tp> operator - (const Vect3D <_Type> &v) const;

    template <typename _Type> inline Vect3D <_Tp> operator * (const _Type a) const; // monozenie
    template <typename _Type> inline Vect3D <_Tp> operator * (const Vect3D <_Type> &v) const;

    template <typename _Type> inline Vect3D <_Tp> operator / (const _Type a) const; // dzielenie
    template <typename _Type> inline Vect3D <_Tp> operator / (const Vect3D <_Type> &v) const;


    // operacje analityczne
    const _Tp& max(void) const; // maksymalna wartosc wektora
    const _Tp& min(void) const; // minimalna wartosc wektora
    const _Tp& max(unsigned& row) const; // maksymalna wartosc wektora dla odpowiedniego wiersza
    const _Tp& min(unsigned& row) const; // minimalna wartosc wektora dla odpowiedniego wiersza

    template <typename _Type> inline _Tp dist(const Vect3D <_Type> &v) const; // odleglosci pomiedzy koncami wektorow

    inline _Tp getNorm(void) const; // obliczanie normalnej wektora
    Vect3D <_Tp> getNormal(void) const; // obliczanie wektora normalnego
    Vect3D <_Tp> getSqrt(void) const; // wyznaczenie pierwiastka kwadratowego wektora

    template <typename _Type> Vect3D <_Tp> toCartesian(const Vect3D<_Type>& origin) const;
    template <typename _Type> Vect3D <_Tp> toGeographic(const Vect3D<_Type>& origin) const;

    template <typename _Type> _Tp getDot(const Vect3D <_Type> &v) const; // iloczyn skalarny
    template <typename _Type> Vect3D <_Tp> getCross(const Vect3D <_Type> &v) const; // iloczyn wektorowy
    template <typename _Type> Vect3D <_Tp> getProj(const Vect3D <_Type>& v) const; // projekcja wektora na wektor v

    Mat3D <_Tp> getCrossProductMatrix(void) const; // macierz krzyzowa wektora

    Vect4D <_Tp> getQuaternion(void) const; // obliczanie kawaternionu
    Mat3D <_Tp> getRotationMatrix(void) const; // obliczanie macierzy kosinusow kierunkowych

    Vect3D <_Tp> getRotedX(const _Tp theta) const; // obliczenie zrotowanego wektora wzgledem osi X o kat theta
    Vect3D <_Tp> getRotedY(const _Tp theta) const; // obliczenie zrotowanego wektora wzgledem osi Y o kat theta
    Vect3D <_Tp> getRotedZ(const _Tp theta) const; // obliczenie zrotowanego wektora wzgledem osi Z o kat theta

    template <typename _Type> Vect3D <_Tp> getRoted(const Vect3D <_Type> &rpy) const; // obliczenie zrotowanego wektora o wektor katow Eulera
    template <typename _Type> Vect3D <_Tp> getRoted(const Vect4D <_Type> &quat) const; // obliczenie zrotowanego wektora o kwaternion
    template <typename _Type> Vect3D <_Tp> getRoted(const Mat3D <_Type> &dcm) const; // obliczenie zrotowanego wektora o macierz kosinusow kierunkowych

    template <typename _Type> Vect3D <_Tp> getRotedTrans(const Vect3D <_Type> &rpy) const; // obliczenie zrotowanego wektora o odwrotny wektor katow Eulera
    template <typename _Type> Vect3D <_Tp> getRotedTrans(const Vect4D <_Type> &quat) const; // obliczenie zrotowanego wektora o odwrotny kwaternion
    template <typename _Type> Vect3D <_Tp> getRotedTrans(const Mat3D <_Type> &dcm) const; // obliczenie zrotowanego wektora o odwrotna macierz kosinusow kierunkowych

    // data validation
    bool isNormal(void) const; // check if data is correct

    // metody statyczne
    static Vect3D <_Tp> ones(void); // wektor jedynkowy
    static Vect3D <_Tp> zeros(void); // wektor zerowy
    static Vect3D <_Tp> gravity(void); // wektor przyspieszenia ziemskiego
    static Vect3D <_Tp> toDeg(const Vect3D<_Tp> &rpyRad); // przeliczenia na stopnie
    static Vect3D <_Tp> toRad(const Vect3D<_Tp> &rpyDeg); // przeliczenie na radiany

    template <typename _Type> static Vect3D <_Tp> eulerFromDCM(const Mat3D<_Type> &R); // konstrukcja wektora katow rpy z macierzy kosinusow kierunkowych
    template <typename _Type> static Vect3D <_Tp> eulerFromQuat(const Vect4D<_Type> &q); // konstrukcja wektora katow rpy z kwaternionu

#ifdef __SKYDIVE_USE_STL__

    template <typename _Type>
    friend std::ostream& operator << (std::ostream& stream, const Vect3D<_Type>& vect);

#endif //__SKYDIVE_USE_STL__
};

template <class _Tp> class Vect4D // wektor 4 - elementowy(kwaternion)
{
public:
    _Tp a, b, c, d; // skladowe wektora

    // konstruktory
    inline Vect4D(void);
    inline Vect4D(_Tp _a, _Tp _b, _Tp _c, _Tp _d);
    explicit Vect4D(const _Tp* vectTab);
    template <typename _Type> inline Vect4D(const Vect4D <_Type> &v);

    // operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
    inline _Tp& operator () (const unsigned row);
    inline const _Tp& operator () (const unsigned row) const; // tylko odczyt

    inline unsigned size() const; // rozmiar wektora


    // operatory logiczne
    template <typename _Type> inline bool operator == (const Vect4D <_Type> &v) const; // rownosc
    template <typename _Type> inline bool operator != (const Vect4D <_Type> &v) const; // nierownosc


    // operatory arytmetyczne
    template <typename _Type> inline Vect4D <_Tp> operator + (const _Type w) const; // suma
    template <typename _Type> inline Vect4D <_Tp> operator + (const Vect4D <_Type> &v) const;

    template <typename _Type> inline Vect4D <_Tp> operator - (const _Type w) const; // rozniaca
    template <typename _Type> inline Vect4D <_Tp> operator - (const Vect4D <_Type> &v) const;

    template <typename _Type> inline Vect4D <_Tp> operator * (const _Type w) const; // monozenie
    template <typename _Type> inline Vect4D <_Tp> operator * (const Vect4D <_Type> &v) const;

    template <typename _Type> inline Vect4D <_Tp> operator / (const _Type w) const; // dzielenie
    template <typename _Type> inline Vect4D <_Tp> operator / (const Vect4D <_Type> &v) const;


    // operacje analityczne
    const _Tp& max(void) const; // maksymalna wartosc wektora
    const _Tp& min(void) const; // minimalna wartosc wektora
    const _Tp& max(unsigned& row) const; // maksymalna wartosc wektora dla odpowiedniego wiersza
    const _Tp& min(unsigned& row) const; // minimalna wartosc wektora dla odpowiedniego wiersza

    template <typename _Type> _Tp inline dist(Vect4D <_Type>& v) const; // odleglosci pomiedzy koncami wektorow

    inline _Tp getNorm(void) const; // obliczanie normalnej wektora
    Vect4D <_Tp> getNormal(void) const; // obliczanie wektora normalnego
    Vect4D <_Tp> getSqrt(void) const; // wyznaczenie pierwiastka kwadratowego wektora

    template <typename _Type> _Tp getDot(const Vect3D <_Type>& v) const; // iloczyn skalarny
    template <typename _Type> Vect4D <_Tp> getCross(const Vect4D <_Type>& q) const; // iloczyn wektorowy kwaternionu
    template <typename _Type> Vect4D <_Tp> getProj(const Vect4D <_Type>& v) const; // projekcja wektora na wektor v

    Mat4D <_Tp> getCrossProductMatrix(void) const; // macierz krzyzowa wektora

    Vect3D <_Tp> getEulerAngles(void) const; // obliczanie katow rotacji Eulera
    Mat3D <_Tp> getDcm(void) const; // obliczanie macierzy kosinusow kierunkowych

    Mat3D <_Tp> getRotedX(const _Tp theta) const; // obliczenie zrotowanego kwaternionu wzgledem osi X o kat theta
    Mat3D <_Tp> getRotedY(const _Tp theta) const; // obliczenie zrotowanego kwaternionu wzgledem osi Y o kat theta
    Mat3D <_Tp> getRotedZ(const _Tp theta) const; // obliczenie zrotowanego kwaternionu wzgledem osi Z o kat theta

    template <typename _Type> Vect4D <_Tp> getRoted(const Vect3D <_Type> &rpy) const; // obliczenie zrotowanego kwaternionu o wektor katow Eulera
    template <typename _Type> Vect4D <_Tp> getRoted(const Vect4D <_Type> &quat) const; // obliczenie zrotowanego kwaternionu o kwaternion
    template <typename _Type> Vect4D <_Tp> getRoted(const Mat3D <_Type> &dcm) const; // obliczenie zrotowanego kwaternionu o macierz kosinusow kierunkowych

    template <typename _Type> Vect4D <_Tp> getRotedTrans(const Vect3D <_Type> &rpy) const; // obliczenie zrotowanego kwaternionu o odwrotny wektor katow Eulera
    template <typename _Type> Vect4D <_Tp> getRotedTrans(const Vect4D <_Type> &quat) const; // obliczenie zrotowanego kwaternionu o odwrotny kwaternion
    template <typename _Type> Vect4D <_Tp> getRotedTrans(const Mat3D <_Type> &dcm) const; // obliczenie zrotowanego kwaternionu o odwrotna macierz kosinusow kierunkowych


    // metody statyczne
    static Vect4D <_Tp> ones(void); // wektor jedynkowy
    static Vect4D <_Tp> zeros(void); // wektor zerowy

    template <typename _Type> static Vect4D <_Tp> quatFromDCM(const Mat3D<_Type> &R);
    template <typename _Type> static Vect4D <_Tp> quatFromEuler(const Vect3D<_Type> &rpy);

#ifdef __SKYDIVE_USE_STL__

    template <typename _Type>
    friend std::ostream& operator << (std::ostream& stream, const Vect4D<_Type>& vect);

#endif //__SKYDIVE_USE_STL__
};


template <class _Tp> class Mat2D // macierz 2x2 (rotacji 1d)
{
public:
    _Tp mat[4];

    // konstruktory
    inline Mat2D(void); // defoultowy (macierz jednostkowa)
    inline Mat2D(const _Tp m0, const _Tp m1, const _Tp m2, const _Tp m3);
    template <typename _Type> inline Mat2D(const Mat2D <_Type> &m);


    // operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
    inline _Tp& operator () (const unsigned row, const unsigned column);
    inline const _Tp& operator () (const unsigned row, const unsigned column) const; // tylko odczyt

    inline Vect2D <_Tp> col(const unsigned column) const;
    inline Vect2D <_Tp> row(const unsigned row) const;
    inline Vect2D <_Tp> diag(void) const;

    inline unsigned size() const; // rozmiar wektora

    const _Tp* begin(void) const; // wskaznik na poczatek tablicy macierzy
    const _Tp* end(void) const; // wskaznik na koniec tablicy macierzy


    // operatory logiczne
    template <typename _Type> inline bool operator == (const Mat2D <_Type> &m) const; // rownosc
    template <typename _Type> inline bool operator != (const Mat2D <_Type> &m) const; // nierownosc


    // operatory arytmetyczne
    template <typename _Type> Mat2D <_Tp> operator * (const _Type a) const; // mnozenie razy stala
    template <typename _Type> Vect2D <_Tp> operator * (const Vect2D <_Type> &v) const; // mnozenie razy wektor
    template <typename _Type> Mat2D <_Tp> operator * (const Mat2D <_Type> &m) const; // mnozenie razy macierz

    template <typename _Type> Vect2D <_Tp> transMul(const Vect2D <_Type> &v) const; // mnozenie transponowanej macierzy razy wektor
    template <typename _Type> Mat2D <_Tp> transMul(const Mat2D <_Type> &m) const; // mnozenie transponowanej macierzy razy macierz

    // operacje analityczne
    _Tp getDet(void) const; // wyznacznik macierzy
    _Tp getTrace(void) const; // slad macierzy
};

template <class _Tp> class Mat3D // macierz 3x3 (rotacji 3d)
{
public:
    _Tp mat[9]; // macierz 3x3 (9-cio elementowa)

    // konstruktory
    inline Mat3D(void); // defoultowy (macierz jednostkowa)
    inline Mat3D(const _Tp m0, const _Tp m1, const _Tp m2, const _Tp m3, const _Tp m4, const _Tp m5, const _Tp m6, const _Tp m7, const _Tp m8);
    template <typename _Type> inline Mat3D(const Mat3D <_Type> &m);

    Mat3D(const Matrix<_Tp>& mat);


    // operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
    inline _Tp& operator () (const unsigned row, const unsigned column);
    inline const _Tp& operator () (const unsigned row, const unsigned column) const; // tylko odczyt

    inline Vect3D <_Tp> col(const unsigned column) const;
    inline Vect3D <_Tp> row(const unsigned row) const;
    inline Vect3D <_Tp> diag(void) const;

    inline unsigned size() const; // rozmiar wektora

    const _Tp* begin(void) const; // wskaznik na poczatek tablicy macierzy
    const _Tp* end(void) const; // wskaznik na koniec tablicy macierzy


    // operatory logiczne
    template <typename _Type> inline bool operator == (const Mat3D <_Type> &m) const; // rownosc
    template <typename _Type> inline bool operator != (const Mat3D <_Type> &m) const; // nierownosc


    // operatory arytmetyczne
    template <typename _Type> Mat3D <_Tp> operator + (const _Type a) const; // dodawanie stalej
    template <typename _Type> Mat3D <_Tp> operator + (const Mat3D <_Type> &m) const; // dodawanie macierzy

    template <typename _Type> Mat3D <_Tp> operator - (const _Type a) const; // odejmowanie stalej
    template <typename _Type> Mat3D <_Tp> operator - (const Mat3D <_Type> &m) const; // odejmowanie macierzy

    template <typename _Type> Mat3D <_Tp> operator * (const _Type a) const; // mnozenie razy stala
    template <typename _Type> Vect3D <_Tp> operator * (const Vect3D <_Type> &v) const; // mnozenie razy wektor
    template <typename _Type> Mat3D <_Tp> operator * (const Mat3D <_Type> &m) const; // mnozenie razy macierz

    template <typename _Type> Vect3D <_Tp> transMul(const Vect3D <_Type> &v) const; // mnozenie transponowanej macierzy razy wektor
    template <typename _Type> Mat3D <_Tp> transMul(const Mat3D <_Type> &m) const; // mnozenie transponowanej macierzy razy macierz

    template <typename _Type> inline Mat3D <_Tp> operator / (const _Type a) const; // dzielenie


    // operacje analityczne
    _Tp getDet(void) const; // wyznacznik macierzy
    _Tp getTrace(void) const; // slad macierzy

    Mat3D <_Tp> getTrans(void) const; // transpozycja macierzy
    Mat3D <_Tp> getInv(void) const; // odwracanie macierzy
    Mat3D <_Tp> getNormal(void) const; // normalizacja macierzy
    Mat3D <_Tp> getQR(Mat3D<_Tp>& R) const; // dekompozycja QR
    Mat3D <_Tp> getEigens(Vect3D <_Tp>& eValues) const; // wyznaczenie wektorow i wartosci wlasnych macierzy
    Mat3D <_Tp> getSqrt(void) const; // wyznaczenie pierwiastka kwadratowego macierzy

    Mat3D <_Tp> getRotedX(const _Tp theta) const; // obliczenie zrotowanej macierzy wzgledem osi X o kat theta
    Mat3D <_Tp> getRotedY(const _Tp theta) const; // obliczenie zrotowanej macierzy wzgledem osi Y o kat theta
    Mat3D <_Tp> getRotedZ(const _Tp theta) const; // obliczenie zrotowanej macierzy wzgledem osi Z o kat theta

    template <typename _Type> Mat3D <_Tp> getRoted(const Vect3D <_Type> &rpy) const; // obliczenie zrotowanej macierzy o wektor katow Eulera
    template <typename _Type> Mat3D <_Tp> getRoted(const Vect4D <_Type> &quat) const; // obliczenie zrotowanej macierzy o kwaternion
    template <typename _Type> Mat3D <_Tp> getRoted(const Mat3D <_Type> &dcm) const; // obliczenie zrotowanej macierzy o macierz kosinusow kierunkowych

    template <typename _Type> Mat3D <_Tp> getRotedTrans(const Vect3D <_Type> &rpy) const; // obliczenie zrotowanej macierzy o odwrotny wektor katow Eulera
    template <typename _Type> Mat3D <_Tp> getRotedTrans(const Vect4D <_Type> &quat) const; // obliczenie zrotowanej macierzy o odwrotny kwaternion
    template <typename _Type> Mat3D <_Tp> getRotedTrans(const Mat3D <_Type> &dcm) const; // obliczenie zrotowanej macierzy o odwrotna macierz kosinusow kierunkowych

    Vect3D <_Tp> getEulerAngles(void) const; // obliczenie wektora katow PRY
    Vect4D <_Tp> getQuaternion(void) const; // obliczenie kwaternionu
    Mat2D <_Tp> getFlatDcm(void) const; // obliczanie macierzy rotacji dla ukladu plaskiego (osie XY, tylko yaw)

    // data validation
    bool isNormal(void) const; // check if data is correct

    // metody statyczne
    static Mat3D <_Tp> eye(void); // macierz jednostkowa
    static Mat3D <_Tp> ones(void); // macierz jedynkowa
    static Mat3D <_Tp> zeros(void); // macierz zerowa
    template <typename _Type> static Mat3D <_Tp> diag(const _Type c); // macierz diagonalna ze stalej
    template <typename _Type> static Mat3D <_Tp> diag(const Vect3D<_Type>& v); // macierz diagonalna z wekora
    template <typename _Type> static Mat3D <_Tp> vectAsRows(const Vect3D <_Type> &v1, const Vect3D <_Type> &v2, const Vect3D <_Type> &v3); // macierz z wektorow na wierszach
    template <typename _Type> static Mat3D <_Tp> vectAsCols(const Vect3D <_Type> &v1, const Vect3D <_Type> &v2, const Vect3D <_Type> &v3); // macierz z wektorow na kolumnach

    template <typename _Type> static Mat3D <_Tp> dcmFromQuat(const Mat4D<_Type> &q); // macierz kosinusow kierunkowych z kwaternionu
    template <typename _Type> static Mat3D <_Tp> dcmFromEuler(const Vect3D<_Type> &rpy); // macierz kosinusow kierunkowych z katow eulera
};

template <class _Tp> class Mat4D // macierz 4x4
{
public:
    _Tp mat[16]; // elementy macierzy

    // konstruktory
    inline Mat4D(void); // defoultowy (macierz jednostkowa)
    inline Mat4D(const _Tp m0, const _Tp m1, const _Tp m2, const _Tp m3, const _Tp m4, const _Tp m5, const _Tp m6, const _Tp m7, const _Tp m8, const _Tp m9, const _Tp m10, const _Tp m11, const _Tp m12, const _Tp m13, const _Tp m14, const _Tp m15);
    template <typename _Type> inline Mat4D(const Mat4D <_Type> &m);

    Mat4D(const Matrix<_Tp>& mat);

    // operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
    inline _Tp& operator () (const unsigned row, const unsigned column);
    inline const _Tp& operator () (const unsigned row, const unsigned column) const; // tylko odczyt

    inline Vect4D <_Tp> col(const unsigned column) const;
    inline Vect4D <_Tp> row(const unsigned row) const;
    inline Vect4D <_Tp> diag(void) const;

    inline unsigned size() const; // rozmiar wektora

    const _Tp* begin(void) const; // wskaznik na poczatek tablicy macierzy
    const _Tp* end(void) const; // wskaznik na koniec tablicy macierzy


    // operatory logiczne
    template <typename _Type> inline bool operator == (const Mat4D <_Type> &m) const; // rownosc
    template <typename _Type> inline bool operator != (const Mat4D <_Type> &m) const; // nierownosc


    // operatory arytmetyczne
    template <typename _Type> Mat4D <_Tp> operator + (const _Type a) const; // dodawanie stalej
    template <typename _Type> Mat4D <_Tp> operator + (const Mat4D <_Type> &m) const; // dodawanie macierzy

    template <typename _Type> Mat4D <_Tp> operator - (const _Type a) const; // odejmowanie stalej
    template <typename _Type> Mat4D <_Tp> operator - (const Mat4D <_Type> &m) const; // odejmowanie macierzy

    template <typename _Type> Mat4D <_Tp> operator * (const _Type a) const; // mnozenie razy stala
    template <typename _Type> Vect4D <_Tp> operator * (const Vect4D <_Type> &v) const; // mnozenie razy wektor
    template <typename _Type> Mat4D <_Tp> operator * (const Mat4D <_Type> &m) const; // mnozenie razy macierz

    template <typename _Type> Vect4D <_Tp> transMul(const Vect4D <_Type> &v) const; // mnozenie transponowanej macierzy razy wektor
    template <typename _Type> Mat4D <_Tp> transMul(const Mat4D <_Type> &m) const; // mnozenie transponowanej macierzy razy macierz

    template <typename _Type> inline Mat4D <_Tp> operator / (const _Type a) const; // dzielenie


    // operacje analityczne
    _Tp getDet(void) const; // wyznacznik macierzy
    _Tp getTrace(void) const;// slad macierzy

    Mat4D <_Tp> getTrans(void) const; // transpozycja macierzy
    Mat4D <_Tp> getInv(void) const; // odwracanie macierzy
    Mat4D <_Tp> getQR(Mat4D<_Tp>& R) const; // dekompozycja QR
    Mat4D <_Tp> getEigens(Vect4D <_Tp>& eValues) const; // wyznaczenie wektorow i wartosci wlasnych macierzy
    Mat4D <_Tp> getSqrt(void) const; // wyznaczenie pierwiastka kwadratowego macierzy


    // metody statyczne
    static Mat4D <_Tp> eye(void); // macierz jednostkowa
    static Mat4D <_Tp> ones(void); // macierz jedynkowa
    static Mat4D <_Tp> zeros(void); // macierz zerowa
    template <typename _Type> static Mat4D <_Tp> diag(const _Type c); // macierz diagonalna ze stalej
    template <typename _Type> static Mat4D <_Tp> diag(const Vect4D<_Type>& v); // macierz diagonalna z wekora
    template <typename _Type> static Mat4D <_Tp> vectAsRows(const Vect4D <_Type> &v1, const Vect4D <_Type> &v2, const Vect4D <_Type> &v3, const Vect4D <_Type> &v4); // macierz z wektorow na wierszach
    template <typename _Type> static Mat4D <_Tp> vectAsCols(const Vect4D <_Type> &v1, const Vect4D <_Type> &v2, const Vect4D <_Type> &v3, const Vect4D <_Type> &v4); // macierz z wektorow na kolumnach
};


template <class _Tp> class Vector
{
    unsigned dataSize;
    _Tp* data;

public:
    Vector(const Vector<_Tp>& v);
    Vector(const unsigned _dataSize);

    Vector(const Vect2D<_Tp>& v);
    Vector(const Vect3D<_Tp>& v);
    Vector(const Vect4D<_Tp>& v);

    ~Vector(void);

    // operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
    _Tp& operator () (const unsigned row);
    _Tp operator () (const unsigned row) const; // tylko odczyt

    unsigned size() const;

    void copyVector(const Vector&  source);

    // operatory arytmetyczne
    template <typename _Type> void operator = (const Vector <_Type>& v) const; // przyrownanie

    template <typename _Type> Vector <_Tp> operator + (const _Type a) const; // dodawanie stalej
    template <typename _Type> Vector <_Tp> operator + (const Vector <_Type>& v) const; // dodawanie wektora


    // metody anlityczne
    void mul(const Matrix<_Tp>& left, const Vector<_Tp>& right);


    // metody stateczne
    static Vector<_Tp> zeros(const unsigned dataSize); // wektor zerowy
    static Vector<_Tp> ones(const unsigned dataSize); // wektor jedynkowy
};

template <class _Tp> class Matrix
{
    unsigned dataSize;
    _Tp* data;

public:
    Matrix(const Matrix<_Tp>& m);
    Matrix(const unsigned _dataSize);

    Matrix(const Mat2D<_Tp>& m);
    Matrix(const Mat3D<_Tp>& m);
    Matrix(const Mat4D<_Tp>& m);

    ~Matrix(void);

    // operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
    _Tp& operator () (const unsigned row, const unsigned column);
    _Tp operator () (const unsigned row, const unsigned column) const;

    unsigned size() const;

    void copyMatrix(const Matrix&  source);

    // metody analityczne
    void mul(const Matrix& left, const Matrix& right);

    void invert();

    // metody statyczne
    static Matrix <_Tp> eye(const unsigned dataSize); // macierz jednostkowa
    static Matrix <_Tp> ones(const unsigned dataSize); // macierz jedynkowa
    static Matrix <_Tp> zeros(const unsigned dataSize); // macierz zerowa
    template <typename _Type> static Matrix <_Tp> diag(const _Type c, const unsigned dataSize); // macierz diagonalna ze stalej
    template <typename _Type> static Matrix <_Tp> diag(const Vector<_Type>& v); // macierz diagonalna z wekora
};

template <typename _Tp> inline _Tp roboLib::toRad(const _Tp deg)
{
    return (_Tp)(deg*(pi / 180));
}
template <typename _Tp> inline _Tp roboLib::toDeg(const _Tp rad)
{
    return (_Tp)((rad*180) / roboLib::pi);
}

template <typename _Tp> inline _Tp roboLib::toKelvin(const _Tp celsius)
{
    return (_Tp)(celsius - roboLib::absoluteZeroCelsius);
}
template <typename _Tp> inline _Tp roboLib::toCelcius(const _Tp kelvin)
{
    return (_Tp)(kelvin + roboLib::absoluteZeroCelsius);
}

template <typename _Tp> inline _Tp roboLib::minmaxVal(const _Tp min, const _Tp max, const _Tp val)
{
    if (val >= min && val <= max)
    {
        return val;
    }
    else
    {
        return val > max ? max : min;
    }
}

template <typename _Tp> inline _Tp roboLib::minmaxVal(const _Tp minmaxAbs, const _Tp val)
{
    return roboLib::minmaxVal<_Tp>(-minmaxAbs, minmaxAbs, val);
}

template <typename _Tp> unsigned short roboLib::valToShort(const _Tp min, const _Tp max, const _Tp val)
{
    const _Tp xCuted = roboLib::minmaxVal(min, max, val) - min;
    return (unsigned short)((USHRT_MAX * xCuted) / fabs(min - max) + 0.5f);
}
template <typename _Tp> _Tp roboLib::shortToVal(const _Tp min, const _Tp max, const unsigned short val)
{
    const _Tp xLenght = fabs(min - max);
    return min + (val * xLenght) / USHRT_MAX;
}


// ================================== Vect2D ===================================
// konstruktory
template <class _Tp>
inline Vect2D <_Tp>::Vect2D(void) :
    x(0), y(0)
{
}
template <class _Tp>
inline Vect2D <_Tp>::Vect2D(_Tp _x, _Tp _y) :
    x(_x), y(_y)
{
}
template <class _Tp>
Vect2D <_Tp>::Vect2D(const _Tp* vectTab)
{
    *this = *(Vect2D<_Tp>*)vectTab;
}
template <class _Tp> template <typename _Type>
inline Vect2D <_Tp>::Vect2D(const Vect2D <_Type> &v) :
    x(_Tp(v.x)), y(_Tp(v.y))
{
}

template <class _Tp>
Vect2D <_Tp>::Vect2D(const Vector<_Tp>& vect)
{
    if (vect.size() >= size())
    {
        x = vect(1);
        y = vect(2);
    }
    else
    {
        *this = Vect2D(); // exeption should be thrown
    }
}

// operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
template <class _Tp>
inline _Tp& Vect2D <_Tp>::operator () (const unsigned row)
{
    switch (row)
    {
    case 1:
        return this->x;
    case 2:
        return this->y;
    default:
        return this->x; // exeption should be thrown
    }
}
template <class _Tp>
inline const _Tp& Vect2D <_Tp>::operator () (const unsigned row) const // tylko odczyt
{
    switch (row)
    {
    case 1:
        return this->x;
    case 2:
        return this->y;
    default:
        return this->x; // exeption should be thrown
    }
}

template <class _Tp>
inline unsigned Vect2D <_Tp>::size() const // rozmiar wektora
{
    return 2;
}

// operatory logiczne
template <class _Tp> template <typename _Type>
inline bool Vect2D <_Tp>::operator == (const Vect2D <_Type> &v) const // rownosc
{
    return x == v.x && y == v.y;
}
template <class _Tp> template <typename _Type>
inline bool Vect2D <_Tp>::operator != (const Vect2D <_Type> &v) const // nierownosc
{
    return x != v.x || y != v.y;
}

// operatory arytmetyczne
template <class _Tp> template <typename _Type>
inline Vect2D<_Tp> Vect2D<_Tp>::operator + (const _Type a) const // suma
{
    return Vect2D <_Tp>(_Tp(x + a), _Tp(y + a));
}
template <class _Tp> template <typename _Type>
inline Vect2D <_Tp> Vect2D <_Tp>::operator + (const Vect2D <_Type> &v) const
{
    return Vect2D <_Tp>(_Tp(x + v.x), _Tp(y + v.y));
}
template <class _Tp> template <typename _Type>
inline Vect2D <_Tp> Vect2D <_Tp>::operator - (const _Type a) const // rozniaca
{
    return Vect2D <_Tp>(_Tp(x - a), _Tp(y - a));
}
template <class _Tp> template <typename _Type>
inline Vect2D <_Tp> Vect2D <_Tp>::operator - (const Vect2D <_Type> &v) const
{
    return Vect2D <_Tp>(_Tp(x - v.x), _Tp(y - v.y));
}

template <class _Tp> template <typename _Type>
inline Vect2D <_Tp> Vect2D <_Tp>::operator * (const _Type a) const // monozenie
{
    return Vect2D <_Tp>(x*(_Tp)a, y*(_Tp)a);
}
template <class _Tp> template <typename _Type>
inline Vect2D <_Tp> Vect2D <_Tp>::operator / (const _Type a) const // dzielenie
{
    return Vect2D <_Tp>(_Tp(x / a), _Tp(y / a));
}
template <class _Tp> template <typename _Type>
inline Vect2D <_Tp> Vect2D <_Tp>::operator * (const Vect2D <_Type> &v) const // monozenie wektorow
{
    return Vect2D <_Tp>(_Tp(x*v.x), _Tp(y*v.y));
}

// operacje analityczne
template <class _Tp>
inline _Tp Vect2D <_Tp>::getNorm(void) const // obliczanie normalnej wektora
{
    return std::sqrt((x)*(x)+(y)*(y));
}
template <class _Tp>
Vect2D <_Tp> Vect2D <_Tp>::getNormal(void) const // obliczanie wektora normalnego
{
    const _Tp n = getNorm();
    return Vect2D <_Tp>(x / n, y / n);
}

template <class _Tp> template <typename _Type>
_Tp Vect2D <_Tp>::getDistance(const Vect2D <_Type> &v) const // wyznaczenie odleglosci miedzy koncami wektorow w ukladzie kartezjanskim
{
    return (_Tp)(std::sqrt((this->x - v.x)*(this->x - v.x) + (this->y - v.y)*(this->y - v.y)));
}
template <class _Tp> template <typename _Type>
_Tp Vect2D <_Tp>::getGeoDistance(const Vect2D <_Type> &geoPoint) const // wyznaczenie odleglosci miedzy koncami wektorow w ukladzie wspolrzednych geograficznych
{
    const _Tp ksi1 = roboLib::toRad(this->x);
    const _Tp ksi2 = roboLib::toRad(geoPoint.x);
    const _Tp dKsi = roboLib::toRad(geoPoint.x - this->x);
    const _Tp dLam = roboLib::toRad(geoPoint.y - this->y);
    const _Tp a = std::sin(dKsi / 2)*std::sin(dKsi / 2) +
            std::cos(ksi1) * std::cos(ksi2) *
            std::sin(dLam / 2)*std::sin(dLam / 2);
    const _Tp c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return (_Tp)(roboLib::rEarth * c);
}

template <class _Tp> template <typename _Type>
Vect2D <_Tp> Vect2D <_Tp>::toCartesian(const Vect2D<_Type>& origin) const
{
    const _Tp latRad = roboLib::toRad(this->x);
    const _Tp lonRad = roboLib::toRad(this->y);
    const _Tp latRad0 = roboLib::toRad(origin.x);
    const _Tp lonRad0 = roboLib::toRad(origin.y);

    return Vect2D <_Tp>
            (
                _Tp(roboLib::rEarth*cos(latRad0)*std::sin(latRad)
                    - roboLib::rEarth*std::cos(latRad)*std::cos(lonRad)*std::cos(lonRad0)*std::sin(latRad0)
                    - roboLib::rEarth*cos(latRad)*std::sin(latRad0)*std::sin(lonRad)*std::sin(lonRad0)),
                _Tp(roboLib::rEarth*std::sin(lonRad - lonRad0)*std::cos(latRad))
                );
}
template <class _Tp> template <typename _Type>
Vect2D <_Tp> Vect2D <_Tp>::toGeographic(const Vect2D<_Type>& origin) const
{
    const _Tp latRad0 = roboLib::toRad(origin.x);
    const _Tp lonRad0 = roboLib::toRad(origin.y);

    const _Tp latRad = _Tp(std::asin((this->x*cos(latRad0)
                                      + roboLib::rEarth*std::sin(latRad0)) / roboLib::rEarth));
    const _Tp lonRad = _Tp(std::atan2(
                               this->y*cos(lonRad0) + roboLib::rEarth*std::cos(latRad0)*std::sin(lonRad0) - this->x*std::sin(latRad0)*std::sin(lonRad0),
                               roboLib::rEarth*std::cos(latRad0)*std::cos(lonRad0) - this->y*std::sin(lonRad0) - this->x*std::cos(lonRad0)*std::sin(latRad0)));

    return Vect2D <_Tp>
            (
                roboLib::toDeg(latRad),
                roboLib::toDeg(lonRad)
                );
}

#ifdef __SKYDIVE_USE_STL__

template <class _Type>
std::ostream& operator << (std::ostream& stream, const Vect2D<_Type>& vect)
{
    stream << "[" << vect.x << ", " << vect.y << "]";
    return stream;
}

#endif //__SKYDIVE_USE_STL__

// ================================== Vect3D ===================================
// konstruktory
template <class _Tp>
inline Vect3D <_Tp>::Vect3D(void) :
    x(0), y(0), z(0)
{
}
template <class _Tp>
inline Vect3D <_Tp>::Vect3D(_Tp _x, _Tp _y, _Tp _z) :
    x(_x), y(_y), z(_z)
{
}
template <class _Tp>
Vect4D <_Tp>::Vect4D(const _Tp* vectTab)
{
    *this = *(Vect4D<_Tp>*)vectTab;
}
template <class _Tp> template <typename _Type>
inline Vect3D <_Tp>::Vect3D(const Vect3D <_Type> &v) :
    x(_Tp(v.x)), y(_Tp(v.y)), z(_Tp(v.z))
{
}
template <class _Tp> template <typename _Type>
inline Vect3D <_Tp>::Vect3D(const Vect2D <_Tp> &v, const _Type z) :
    x(_Tp(v.x)), y(_Tp(v.y)), z(_Tp(z))
{
}

template <class _Tp>
Vect3D <_Tp>::Vect3D(const Vector<_Tp>& vect)
{
    if (vect.size() >= size())
    {
        x = vect(1);
        y = vect(2);
        z = vect(3);
    }
    else
    {
        *this = Vect3D(); // exeption should be thrown
    }
}

// operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
template <class _Tp>
inline _Tp& Vect3D <_Tp>::operator () (const unsigned row)
{
    switch (row)
    {
    case 1:
        return this->x;
    case 2:
        return this->y;
    case 3:
        return this->z;
    default:
        return this->x; // exeption should be thrown
    }
}
template <class _Tp>
inline const _Tp& Vect3D <_Tp>::operator () (const unsigned row) const // tylko odczyt
{
    switch (row)
    {
    case 1:
        return this->x;
    case 2:
        return this->y;
    case 3:
        return this->z;
    default:
        return this->x; // exeption should be thrown
    }
}

template <class _Tp>
Vect2D<_Tp> Vect3D <_Tp>::getVect2D(void) const // wektora 2D z wspolrzednych x i y wektora 3D
{
    return Vect2D<_Tp>(this->x, this->y);
}

template <class _Tp>
inline unsigned Vect3D <_Tp>::size() const // rozmiar wektora
{
    return 3;
}

// operatory logiczne
template <class _Tp> template <typename _Type>
inline bool Vect3D <_Tp>::operator == (const Vect3D <_Type> &v) const // rownosc
{
    return x == v.x && y == v.y && z == v.z;
}
template <class _Tp> template <typename _Type>
inline bool Vect3D <_Tp>::operator != (const Vect3D <_Type> &v) const // nierownosc
{
    return x != v.x || y != v.y || z != v.z;
}

// operatory arytmetyczne
template <class _Tp> template <typename _Type>
inline Vect3D<_Tp> Vect3D<_Tp>::operator + (const _Type a) const // suma
{
    return Vect3D <_Tp>(_Tp(x + a), _Tp(y + a), _Tp(z + a));
}
template <class _Tp> template <typename _Type>
inline Vect3D <_Tp> Vect3D <_Tp>::operator + (const Vect3D <_Type> &v) const
{
    return Vect3D <_Tp>(_Tp(x + v.x), _Tp(y + v.y), _Tp(z + v.z));
}
template <class _Tp> template <typename _Type>
inline Vect3D <_Tp> Vect3D <_Tp>::operator - (const _Type a) const // rozniaca
{
    return Vect3D <_Tp>(_Tp(x - a), _Tp(y - a), _Tp(z - a));
}
template <class _Tp> template <typename _Type>
inline Vect3D <_Tp> Vect3D <_Tp>::operator - (const Vect3D <_Type> &v) const
{
    return Vect3D <_Tp>(_Tp(x - v.x), _Tp(y - v.y), _Tp(z - v.z));
}

template <class _Tp> template <typename _Type>
inline Vect3D <_Tp> Vect3D <_Tp>::operator * (const _Type a) const // monozenie
{
    return Vect3D <_Tp>(_Tp(x*a), _Tp(y*a), _Tp(z*a));
}
template <class _Tp> template <typename _Type>
inline Vect3D <_Tp> Vect3D <_Tp>::operator / (const _Type a) const // dzielenie
{
    return Vect3D <_Tp>(_Tp(x / a), _Tp(y / a), _Tp(z / a));
}
template <class _Tp> template <typename _Type>
inline Vect3D <_Tp> Vect3D <_Tp>::operator * (const Vect3D <_Type> &v) const // monozenie wektorow
{
    return Vect3D <_Tp>(_Tp(x*v.x), _Tp(y*v.y), _Tp(z*v.z));
}

// operacje analityczne
template <class _Tp>
const _Tp& Vect3D <_Tp>::max(void) const // maksymalna wartosc wektora
{
    if (x > y)
    {
        if (x > z) return x;
        else return z;
    }
    else
    {
        if (y > z) return y;
        else return z;
    }
}
template <class _Tp>
const _Tp& Vect3D <_Tp>::min(void) const // minimalna wartosc wektora
{
    if (x < y)
    {
        if (x < z) return x;
        else return z;
    }
    else
    {
        if (y < z) return y;
        else return z;
    }
}
template <class _Tp>
const _Tp& Vect3D <_Tp>::max(unsigned& row) const // maksymalna wartosc wektora
{
    if (x > y)
    {
        if (x > z)
        {
            row = 1;
            return x;
        }
        else
        {
            row = 3;
            return z;
        }
    }
    else
    {
        if (y > z)
        {
            row = 2;
            return y;
        }
        else
        {
            row = 3;
            return z;
        }
    }
}
template <class _Tp>
const _Tp& Vect3D <_Tp>::min(unsigned& row) const // minimalna wartosc wektora
{
    if (x < y)
    {
        if (x < z)
        {
            row = 1;
            return x;
        }
        else
        {
            row = 3;
            return z;
        }
    }
    else
    {
        if (y < z)
        {
            row = 2;
            return y;
        }
        else
        {
            row = 3;
            return z;
        }
    }
}

template <class _Tp> template <typename _Type>
inline _Tp Vect3D <_Tp>::dist(const Vect3D <_Type> &v) const // odleglosc punktu od punktu
{
    return sqrt(_Tp((x - v.x)*(x - v.x)) + _Tp((y - v.y)*(y - v.y)) + _Tp((z - v.z)*(z - v.z)));
}

template <class _Tp>
inline _Tp Vect3D <_Tp>::getNorm(void) const // obliczanie normalnej wektora
{
    return sqrt((x)*(x)+(y)*(y)+(z)*(z));
}
template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::getNormal(void) const // obliczanie wektora normalnego
{
    const _Tp n = getNorm();
    return Vect3D <_Tp>(x / n, y / n, z / n);
}
template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::getSqrt(void) const // wyznaczenie pierwiastka kwadratowego wektora
{
    return Vect3D <_Tp>(sqrt(x), sqrt(y), sqrt(z));
}

template <class _Tp> template <typename _Type>
Vect3D <_Tp> Vect3D <_Tp>::toCartesian(const Vect3D<_Type>& origin) const
{
    return Vect3D <_Tp>
            (
                getVect2D().toCartesian(origin),
                _Tp(this->z - origin.z)
                );
}
template <class _Tp> template <typename _Type>
Vect3D <_Tp> Vect3D <_Tp>::toGeographic(const Vect3D<_Type>& origin) const
{
    return Vect3D <_Tp>
            (
                getVect2D().toGeographic(origin),
                _Tp(origin.z + this->z)
                );
}

template <class _Tp> template <typename _Type>
_Tp Vect3D <_Tp>::getDot(const Vect3D <_Type> &v) const // iloczyn skalarny
{
    return _Tp(x*v.x + y*v.y + z*v.z);
}
template <class _Tp> template <typename _Type>
Vect3D <_Tp> Vect3D <_Tp>::getCross(const Vect3D <_Type> &v) const // iloczyn wektorowy
{
    return Vect3D <_Tp>
            (
                _Tp(v.z*y - v.y*z),
                _Tp(v.x*z - v.z*x),
                _Tp(v.y*x - v.x*y)
                );
}
template <class _Tp>  template <typename _Type>
Vect3D <_Tp> Vect3D <_Tp>::getProj(const Vect3D <_Type>& v) const // projekcja wektora na wektor v
{
    return Vect3D <_Tp>(v*(this->getDot(v) / v.getDot(v)));
}

template <class _Tp>
Mat3D <_Tp> Vect3D <_Tp>::getCrossProductMatrix(void) const // macierz uogolniona wektora
{
    return Mat3D <_Tp>
            (
                0, -z, y,
                z, 0, -x,
                -y, x, 0
                );
}

template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::getRotedX(const _Tp theta) const // obliczenie zrotowanego wektora wzgledem osi X o kat theta
{
    return Vect3D <_Tp>();
}
template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::getRotedY(const _Tp theta) const // obliczenie zrotowanego wektora wzgledem osi Y o kat theta
{
    return Vect3D <_Tp>();
}
template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::getRotedZ(const _Tp theta) const // obliczenie zrotowanego wektora wzgledem osi Z o kat theta
{
    return Vect3D <_Tp>();
}

// data validation
template <class _Tp>
bool Vect3D <_Tp>::isNormal(void) const // check if data is correct
{
    return std::isnormal(this->x) && std::isnormal(this->y) && std::isnormal(this->z);
}

// metody statyczne
template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::ones(void) // wektor jedynkowy
{
    return Vect3D <_Tp>(1, 1, 1);
}
template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::zeros(void) // wektor zerowy
{
    return Vect3D <_Tp>(0, 0, 0);
}
template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::gravity(void) // wektor przyspieszenia ziemskiego
{
    return Vect3D <_Tp>(0, 0, _Tp(roboLib::gravity));
}

template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::toDeg(const Vect3D<_Tp> &rpyRad) // przeliczenia na stopnie
{
    return Vect3D <_Tp>
            (
                roboLib::toDeg(rpyRad.x),
                roboLib::toDeg(rpyRad.y),
                roboLib::toDeg(rpyRad.z)
                );
}
template <class _Tp>
Vect3D <_Tp> Vect3D <_Tp>::toRad(const Vect3D<_Tp> &rpyDeg) // przeliczenie na radiany
{
    return Vect3D <_Tp>
            (
                roboLib::toRad(rpyDeg.x),
                roboLib::toRad(rpyDeg.y),
                roboLib::toRad(rpyDeg.z)
                );
}

#ifdef __SKYDIVE_USE_STL__

template <class _Type>
std::ostream& operator << (std::ostream& stream, const Vect3D<_Type>& vect)
{
    stream << "[" << vect.x << ", " << vect.y << ", " << vect.z << "]";
    return stream;
}

#endif //__SKYDIVE_USE_STL__


// ================================== Vect4D ===================================
// konstruktory
template <class _Tp>
inline Vect4D <_Tp>::Vect4D(void) :
    a(0), b(0), c(0), d(0)
{
}
template <class _Tp>
inline Vect4D <_Tp>::Vect4D(_Tp _a, _Tp _b, _Tp _c, _Tp _d) :
    a(_a), b(_b), c(_c), d(_d)
{
}
template <class _Tp>
Vect3D <_Tp>::Vect3D(const _Tp* vectTab)
{
    *this = *(Vect3D<_Tp>*)vectTab;
}
template <class _Tp> template <typename _Type>
inline Vect4D <_Tp>::Vect4D(const Vect4D <_Type> &v) :
    a(_Tp(v.a)), b(_Tp(v.b)), c(_Tp(v.c)), d(_Tp(v.d))
{
}

// operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
template <class _Tp>
inline _Tp& Vect4D <_Tp>::operator () (const unsigned row)
{
    switch (row)
    {
    case 1:
        return this->a;
    case 2:
        return this->b;
    case 3:
        return this->c;
    case 4:
        return this->d;
    default:
        return this->a; // exeption should be thrown
    }
}
template <class _Tp>
inline const _Tp& Vect4D <_Tp>::operator () (const unsigned row) const // tylko odczyt
{
    switch (row)
    {
    case 1:
        return this->a;
    case 2:
        return this->b;
    case 3:
        return this->c;
    case 4:
        return this->d;
    default:
        return this->a; // exeption should be thrown
    }
}

template <class _Tp>
inline unsigned Vect4D <_Tp>::size() const // rozmiar wektora
{
    return 4;
}

// operatory logiczne
template <class _Tp> template <typename _Type>
inline bool Vect4D <_Tp>::operator == (const Vect4D <_Type> &v) const // rownosc
{
    return a == v.a && b == v.b && c == v.c && d == v.d;
}
template <class _Tp> template <typename _Type>
inline bool Vect4D <_Tp>::operator != (const Vect4D <_Type> &v) const // nierownosc
{
    return a != v.a || b != v.b || c != v.c || d != v.d;
}

// operatory arytmetyczne
template <class _Tp> template <typename _Type>
inline Vect4D <_Tp> Vect4D <_Tp>::operator + (const _Type w) const // suma
{
    return Vect4D <_Tp>(_Tp(a + w), _Tp(b + w), _Tp(c + w), _Tp(d + w));
}
template <class _Tp> template <typename _Type>
inline Vect4D <_Tp> Vect4D <_Tp>::operator + (const Vect4D <_Type> &v) const
{
    return Vect4D <_Tp>(_Tp(a + v.a), _Tp(b + v.b), _Tp(c + v.c), _Tp(d + v.d));
}

template <class _Tp> template <typename _Type>
inline Vect4D <_Tp> Vect4D <_Tp>::operator - (const _Type w) const // rozniaca
{
    return Vect4D <_Tp>(_Tp(a - w), _Tp(b - w), _Tp(c - w), _Tp(d - w));
}
template <class _Tp> template <typename _Type>
inline Vect4D <_Tp> Vect4D <_Tp>::operator - (const Vect4D <_Type> &v) const
{
    return Vect4D <_Tp>(_Tp(a - v.a), _Tp(b - v.b), _Tp(c - v.c), _Tp(d - v.d));
}

template <class _Tp> template <typename _Type>
inline Vect4D <_Tp> Vect4D <_Tp>::operator * (const _Type w) const // monozenie
{
    return Vect4D <_Tp>(_Tp(a * w), _Tp(b * w), _Tp(c * w), _Tp(d * w));
}
template <class _Tp> template <typename _Type>
inline Vect4D <_Tp> Vect4D <_Tp>::operator / (const _Type w) const // dzielenie
{
    return Vect4D <_Tp>(_Tp(a / w), _Tp(b / w), _Tp(c / w), _Tp(d / w));
}

// operacje analityczne
template <class _Tp> template <typename _Type>
_Tp inline Vect4D <_Tp>::dist(Vect4D <_Type> &v) const
{
    return std::sqrt(_Tp((a - v.a)*(a - v.a) + (b - v.b)*(b - v.b) + (c - v.c)*(c - v.c) + (d - v.d)*(d - v.d)));
}

template <class _Tp>
inline _Tp Vect4D <_Tp>::getNorm(void) const // obliczanie normalnej wektora
{
    return std::sqrt(a*a + b*b + c*c + d*d);
}
template <class _Tp>
Vect4D <_Tp> Vect4D <_Tp>::getNormal(void) const // obliczanie wektora normalnego
{
    const _Tp n = this->getNorm();
    return Vect4D <_Tp>(a / n, b / n, c / n, d / n);
}

template <class _Tp> template <typename _Type>
_Tp Vect4D <_Tp>::getDot(const Vect3D <_Type> &v) const // iloczyn skalarny
{
    return _Tp(v.a*a + v.b*b + v.c*c + v.d*d);
}

template <class _Tp>
Mat4D <_Tp> Vect4D <_Tp>::getCrossProductMatrix(void) const // macierz krzyzowa wektora
{
    return Mat3D <_Tp>();
}

template <class _Tp> template <typename _Type>
Vect4D <_Tp> Vect4D <_Tp>::getCross(const Vect4D <_Type> &q) const // iloczyn wektorowy kwaternionu
{
    return Vect4D <_Tp>
            (

                );
}

template <class _Tp>
Vect3D <_Tp> Vect4D <_Tp>::getEulerAngles(void) const // obliczanie katow rotacji Eulera
{
    return Vect3D <_Tp>
            (
                std::atan2(2 * (b*c + a*d), d*d + c*c - b*b - a*a),
                std::asin(-2 * (a*c - b*d)),
                std::atan2(2 * (a*b + d*c), d*d + a*a - b*b - c*c)
                );
}
template <class _Tp>
Mat3D <_Tp> Vect4D <_Tp>::getDcm(void) const // obliczanie macierzy kosinusow kierunkowych
{
    return Mat3D <_Tp>
            (
                d*d + a*a - b*b - c*c,
                2 * (a*b + d*c),
                2 * (a*c - d*b),
                2 * (a*b - d*c),
                d*d - a*a + b*b - c*c,
                2 * (b*c + d*a),
                2 * (a*c + d*b),
                2 * (b*c - d*a),
                d*d - a*a - b*b + c*c
                );
}

// metody statyczne
template <class _Tp> template <typename _Type>
Vect4D <_Tp> Vect4D <_Tp>::quatFromDCM(const Mat3D<_Type> &dcm)
{
    return dcm.getQuaternion();
}
template <class _Tp> template <typename _Type>
Vect4D <_Tp> Vect4D <_Tp>::quatFromEuler(const Vect3D<_Type> &rpy)
{
    return Mat3D<_Type>::dcmFromEuler(rpy).getQuaternion();
}

#ifdef __SKYDIVE_USE_STL__

template <class _Type>
std::ostream& operator << (std::ostream& stream, const Vect4D<_Type>& vect)
{
    stream << "[" << vect.a << ", " << vect.b << ", " << vect.c << ", " << vect.d << "]";
    return stream;
}

#endif //__SKYDIVE_USE_STL__

// ================================== Mat2D ====================================
// konstruktory
template <class _Tp>
inline Mat2D <_Tp>::Mat2D(void) // defoultowy (macierz jednostkowa)
{
    mat[0] = 1; mat[1] = 0;
    mat[2] = 0; mat[3] = 1;
}
template <class _Tp>
inline Mat2D <_Tp>::Mat2D(const _Tp m0, const _Tp m1, const _Tp m2, const _Tp m3) // defoultowy (macierz jednostkowa)
{
    mat[0] = m0; mat[1] = m1;
    mat[2] = m2; mat[3] = m3;
}

// operatory arytmetyczne
template <class _Tp> template <typename _Type>
Vect2D <_Tp> Mat2D <_Tp>::operator * (const Vect2D <_Type> &v) const // mnozenie razy wektor
{
    return Vect2D <_Tp>
            (
                _Tp(mat[0] * v.x + mat[1] * v.y),
            _Tp(mat[2] * v.x + mat[3] * v.y)
            );
}

template <class _Tp> template <typename _Type>
Vect2D <_Tp> Mat2D <_Tp>::transMul(const Vect2D <_Type> &v) const // mnozenie razy wektor
{
    return Vect2D <_Tp>
            (
                _Tp(mat[0] * v.x + mat[2] * v.y),
            _Tp(mat[1] * v.x + mat[3] * v.y)
            );
}


// ================================== Mat3D ====================================
// konstruktory
template <class _Tp>
inline Mat3D <_Tp>::Mat3D(void) // defaultowy (macierz jednostkowa)
{
    mat[0] = 1; mat[1] = 0; mat[2] = 0;
    mat[3] = 0; mat[4] = 1; mat[5] = 0;
    mat[6] = 0; mat[7] = 0; mat[8] = 1;
}
template <class _Tp>
inline Mat3D <_Tp>::Mat3D(const _Tp m0, const _Tp m1, const _Tp m2, const _Tp m3, const _Tp m4, const _Tp m5, const _Tp m6, const _Tp m7, const _Tp m8)
{
    mat[0] = m0; mat[1] = m1; mat[2] = m2;
    mat[3] = m3; mat[4] = m4; mat[5] = m5;
    mat[6] = m6; mat[7] = m7; mat[8] = m8;
}
template <class _Tp> template <typename _Type>
inline Mat3D <_Tp>::Mat3D(const Mat3D <_Type> &m)
{
    mat[0] = _Tp(m.mat[0]); mat[1] = _Tp(m.mat[1]); mat[2] = _Tp(m.mat[2]);
    mat[3] = _Tp(m.mat[3]); mat[4] = _Tp(m.mat[4]); mat[5] = _Tp(m.mat[5]);
    mat[6] = _Tp(m.mat[6]); mat[7] = _Tp(m.mat[7]); mat[8] = _Tp(m.mat[8]);
}

template <class _Tp>
Mat3D <_Tp>::Mat3D(const Matrix<_Tp>& matrix)
{
    if (matrix.size() >= size())
    {
        for (unsigned i = 0; i < size(); i++)
        {
            for (unsigned j = 0; j < size(); j++)
            {
                mat[i * size() + j] = matrix(i + 1, j + 1);
            }
        }
    }
    else
    {
        *this = Mat3D(); // exeption should be thrown
    }
}

// operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
template <class _Tp>
inline _Tp& Mat3D <_Tp>::operator () (const unsigned row, const unsigned column)
{
    if (!((row - 1 >= 3) || (column - 1 >= 3) || (row - 1 < 0) || (column - 1 < 0)))
    {
        return mat[(row - 1) * 3 + (column - 1)];
    }
    else return mat[0]; // exeption should be thrown
}
template <class _Tp>
inline const _Tp& Mat3D <_Tp>::operator () (const unsigned row, const unsigned column) const
{
    if (!((row - 1 >= 3) || (column - 1 >= 3) || (row - 1 < 0) || (column - 1 < 0)))
    {
        return mat[(row - 1) * 3 + (column - 1)];
    }
    else return mat[0]; // exeption should be thrown
}

template <class _Tp>
inline Vect3D <_Tp> Mat3D <_Tp>::col(const unsigned column) const // pobranie odpowiedniej kolumny macierzy
{
    return Vect3D <_Tp> // (row - 1)*size + (col - 1), size = 3, row = 1,2,...,size
            (
                mat[column - 1],
            mat[column + 2],
            mat[column + 5]
            );
}
template <class _Tp>
inline Vect3D <_Tp> Mat3D <_Tp>::row(const unsigned row) const // pobranie odpowiedniego rzedu macierzy
{
    return Vect3D <_Tp> // (row - 1)*size + (col - 1), size = 3, col = 1,2,...,size
            (
                mat[3 * row - 3],
            mat[3 * row - 2],
            mat[3 * row - 1]
            );
}
template <class _Tp>
inline Vect3D <_Tp> Mat3D <_Tp>::diag(void) const // pobranie przekatniej macierzy
{
    return Vect3D <_Tp>
            (
                mat[0],
            mat[4],
            mat[8]
            );
}

template <class _Tp>
inline unsigned Mat3D <_Tp>::size() const // rozmiar macierzy
{
    return 3;
}


template <class _Tp>
const _Tp* Mat3D <_Tp>::begin(void) const // wskaznik na poczatek tablicy macierzy
{
    return mat;
}
template <class _Tp>
const _Tp* Mat3D <_Tp>::end(void) const // wskaznik na koniec tablicy macierzy
{
    return mat + 9;
}


// operatory logiczne
template <class _Tp> template <typename _Type>
inline bool Mat3D <_Tp>::operator == (const Mat3D <_Type> &m) const // rownosc
{
    return
            mat[0] == m.mat[0] && mat[1] == m.mat[1] && mat[2] == m.mat[2] &&
            mat[3] == m.mat[3] && mat[4] == m.mat[4] && mat[5] == m.mat[5] &&
            mat[6] == m.mat[6] && mat[7] == m.mat[7] && mat[8] == m.mat[8];
}
template <class _Tp> template <typename _Type>
inline bool Mat3D <_Tp>::operator != (const Mat3D <_Type> &m) const // nierownosc
{
    return
            mat[0] != m.mat[0] || mat[1] != m.mat[1] || mat[2] != m.mat[2] ||
            mat[3] != m.mat[3] || mat[4] != m.mat[4] || mat[5] != m.mat[5] ||
            mat[6] != m.mat[6] || mat[7] != m.mat[7] || mat[8] != m.mat[8];
}

// operacje analityczne
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::operator + (const _Type a) const // dodawanie stalej
{
    return Mat3D <_Tp>
            (
                _Tp(mat[0] + a), _Tp(mat[1] + a), _Tp(mat[2] + a),
            _Tp(mat[3] + a), _Tp(mat[4] + a), _Tp(mat[5] + a),
            _Tp(mat[6] + a), _Tp(mat[7] + a), _Tp(mat[8] + a)
            );
}
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::operator + (const Mat3D <_Type> &m) const // dodawanie macierzy
{
    return Mat3D <_Tp>
            (
                _Tp(mat[0] + m.mat[0]), _Tp(mat[1] + m.mat[1]), _Tp(mat[2] + m.mat[2]),
            _Tp(mat[3] + m.mat[3]), _Tp(mat[4] + m.mat[4]), _Tp(mat[5] + m.mat[5]),
            _Tp(mat[6] + m.mat[6]), _Tp(mat[7] + m.mat[7]), _Tp(mat[8] + m.mat[8])
            );
}

template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::operator - (const _Type a) const // odejmowanie stalej
{
    return Mat3D <_Tp>
            (
                _Tp(mat[0] - a), _Tp(mat[1] - a), _Tp(mat[2] - a),
            _Tp(mat[3] - a), _Tp(mat[4] - a), _Tp(mat[5] - a),
            _Tp(mat[6] - a), _Tp(mat[7] - a), _Tp(mat[8] - a)
            );
}
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::operator - (const Mat3D <_Type> &m) const // odejmowanie macierzy
{
    return Mat3D <_Tp>
            (
                _Tp(mat[0] - m.mat[0]), _Tp(mat[1] - m.mat[1]), _Tp(mat[2] - m.mat[2]),
            _Tp(mat[3] - m.mat[3]), _Tp(mat[4] - m.mat[4]), _Tp(mat[5] - m.mat[5]),
            _Tp(mat[6] - m.mat[6]), _Tp(mat[7] - m.mat[7]), _Tp(mat[8] - m.mat[8])
            );
}

template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::operator * (const _Type a) const // mnozenie razy stala
{
    return Mat3D <_Tp>
            (
                _Tp(mat[0] * a), _Tp(mat[1] * a), _Tp(mat[2] * a),
            _Tp(mat[3] * a), _Tp(mat[4] * a), _Tp(mat[5] * a),
            _Tp(mat[6] * a), _Tp(mat[7] * a), _Tp(mat[8] * a)
            );
}
template <class _Tp> template <typename _Type>
Vect3D <_Tp> Mat3D <_Tp>::operator * (const Vect3D <_Type> &v) const // mnozenie razy wektor
{
    return Vect3D <_Tp>
            (
                _Tp(mat[0] * v.x + mat[1] * v.y + mat[2] * v.z),
            _Tp(mat[3] * v.x + mat[4] * v.y + mat[5] * v.z),
            _Tp(mat[6] * v.x + mat[7] * v.y + mat[8] * v.z)
            );
}
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::operator * (const Mat3D <_Type> &m) const // mnozenie razy macierz
{
    return Mat3D <_Tp>
            (
                _Tp(mat[0] * m.mat[0] + mat[1] * m.mat[3] + mat[2] * m.mat[6]),
            _Tp(mat[0] * m.mat[1] + mat[1] * m.mat[4] + mat[2] * m.mat[7]),
            _Tp(mat[0] * m.mat[2] + mat[1] * m.mat[5] + mat[2] * m.mat[8]),
            _Tp(mat[3] * m.mat[0] + mat[4] * m.mat[3] + mat[5] * m.mat[6]),
            _Tp(mat[3] * m.mat[1] + mat[4] * m.mat[4] + mat[5] * m.mat[7]),
            _Tp(mat[3] * m.mat[2] + mat[4] * m.mat[5] + mat[5] * m.mat[8]),
            _Tp(mat[6] * m.mat[0] + mat[7] * m.mat[3] + mat[8] * m.mat[6]),
            _Tp(mat[6] * m.mat[1] + mat[7] * m.mat[4] + mat[8] * m.mat[7]),
            _Tp(mat[6] * m.mat[2] + mat[7] * m.mat[5] + mat[8] * m.mat[8])
            );
}

template <class _Tp> template <typename _Type>
Vect3D <_Tp> Mat3D <_Tp>::transMul(const Vect3D <_Type> &v) const // mnozenie razy wektor
{
    return Vect3D <_Tp>
            (
                _Tp(mat[0] * v.x + mat[3] * v.y + mat[6] * v.z),
            _Tp(mat[1] * v.x + mat[4] * v.y + mat[7] * v.z),
            _Tp(mat[2] * v.x + mat[5] * v.y + mat[8] * v.z)
            );
}
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::transMul(const Mat3D <_Type> &m) const // mnozenie razy macierz
{
    return Mat3D <_Tp>
            (
                _Tp(m.mat[0] * mat[0] + m.mat[3] * mat[3] + m.mat[6] * mat[6]),
            _Tp(m.mat[1] * mat[0] + m.mat[4] * mat[3] + m.mat[7] * mat[6]),
            _Tp(m.mat[2] * mat[0] + m.mat[5] * mat[3] + m.mat[8] * mat[6]),
            _Tp(m.mat[0] * mat[1] + m.mat[3] * mat[4] + m.mat[6] * mat[7]),
            _Tp(m.mat[1] * mat[1] + m.mat[4] * mat[4] + m.mat[7] * mat[7]),
            _Tp(m.mat[2] * mat[1] + m.mat[5] * mat[4] + m.mat[8] * mat[7]),
            _Tp(m.mat[0] * mat[2] + m.mat[3] * mat[5] + m.mat[6] * mat[8]),
            _Tp(m.mat[1] * mat[2] + m.mat[4] * mat[5] + m.mat[7] * mat[8]),
            _Tp(m.mat[2] * mat[2] + m.mat[5] * mat[5] + m.mat[8] * mat[8])
            );
}

template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::operator / (const _Type a) const // dzielenie przez stala
{
    return Mat3D <_Tp>
            (
                _Tp(mat[0] / a), _Tp(mat[1] / a), _Tp(mat[2] / a),
            _Tp(mat[3] / a), _Tp(mat[4] / a), _Tp(mat[5] / a),
            _Tp(mat[6] / a), _Tp(mat[7] / a), _Tp(mat[8] / a)
            );
}

template <class _Tp>
_Tp Mat3D <_Tp>::getDet(void) const // wyznacznik macierzy
{
    return mat[0] * mat[4] * mat[8] + mat[3] * mat[7] * mat[2] + mat[1] * mat[5] * mat[6] - mat[2] * mat[4] * mat[6] - mat[5] * mat[7] * mat[0] - mat[8] * mat[1] * mat[3];
}
template <class _Tp>
_Tp Mat3D <_Tp>::getTrace(void) const // slad macierzy
{
    return mat[0] + mat[4] + mat[8];
}

template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getTrans(void) const // transpozycja macierzy
{
    return Mat3D <_Tp>
            (
                mat[0], mat[3], mat[6],
            mat[1], mat[4], mat[7],
            mat[2], mat[5], mat[8]
            );
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getInv(void) const // odwracanie macierzy
{
    const _Tp d = this->getDet();
    return Mat3D <_Tp>
            (
                (mat[4] * mat[8] - mat[5] * mat[7]) / d, -(mat[1] * mat[8] - mat[2] * mat[7]) / d, (mat[1] * mat[5] - mat[2] * mat[4]) / d,
            -(mat[3] * mat[8] - mat[5] * mat[6]) / d, (mat[0] * mat[8] - mat[2] * mat[6]) / d, -(mat[0] * mat[5] - mat[2] * mat[3]) / d,
            (mat[3] * mat[7] - mat[4] * mat[6]) / d, -(mat[0] * mat[7] - mat[1] * mat[6]) / d, (mat[0] * mat[4] - mat[1] * mat[3]) / d
            );
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getNormal(void) const // normalizacja macierzy
{
    const Vect3D<_Tp> X(mat[0], mat[3], mat[6]);
    const Vect3D<_Tp> Y(mat[1], mat[4], mat[7]);

    const _Tp error = X.getDot(Y);

    const Vect3D<_Tp> X_orth = X - Y*(error / 2);
    const Vect3D<_Tp> Y_orth = Y - X*(error / 2);
    const Vect3D<_Tp> Z_orth = X.getCross(Y);

    const Vect3D<_Tp> X_norm = X_orth * 0.5*(3 - X_orth.getDot(X_orth));
    const Vect3D<_Tp> Y_norm = Y_orth * 0.5*(3 - Y_orth.getDot(Y_orth));
    const Vect3D<_Tp> Z_norm = Z_orth * 0.5*(3 - Z_orth.getDot(Z_orth));

    return Mat3D <_Tp>::vectAsCols
            (
                X_norm, Y_norm, Z_norm
                );
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getQR(Mat3D<_Tp>& R) const // dekompozycja QR
{
    const Vect3D <_Tp> x1 = this->col(1);
    const Vect3D <_Tp> x2 = this->col(2);
    const Vect3D <_Tp> x3 = this->col(3);

    // wyznaczenie macierzy ortogonalnej metoda Grama - Schmidta
    const Vect3D <_Tp> v2 = x2 - x2.getProj(x1);
    const Vect3D <_Tp> v3 = x3 - x3.getProj(x1) - x3.getProj(v2);

    // ortonormalizacja
    const Vect3D <_Tp> v1n = x1.getNormal();
    const Vect3D <_Tp> v2n = v2.getNormal();
    const Vect3D <_Tp> v3n = v3.getNormal();

    // wyznacznie macierzy Q i R
    const Mat3D <_Tp> Q = Mat3D <_Tp>::vectAsCols(v1n, v2n, v3n);
    R = Q.transMul(*this);

    return Q;
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getEigens(Vect3D <_Tp>& eValues) const // wyznaczenie wektorow i wartosci wlasnych macierzy
{
    // wyznaczenie wektorow i warosci wlasnych macierzy 3x3 przy pomocy algorytmu QR
    Mat3D <_Tp> eVectors = Mat3D <_Tp>::eye();
    Mat3D <_Tp> eVals = *this;
    for (unsigned i = 0; i < MAX_QR_ALGORITHM_ITERATIONS; i++)
    {
        Mat3D <_Tp> R;
        Mat3D <_Tp> Q = eVals.getQR(R); // dekompozycja QR
        eVals = R*Q; // aktualizacja macierzy
        eVectors = eVectors*Q;
    }
    eValues = eVals.diag();
    return eVectors;
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getSqrt(void) const // wyznaczenie pierwiastka kwadratowego macierzy
{
    // metoda diagonalizacji
    // rozwiazywanie problemu wlasnego macierzy
    Vect3D <_Tp> eVals;
    Mat3D <_Tp> eVects = this->getEigens(eVals);
    // wyznaczanie pierwiastka kwadratowego
    Mat3D <_Tp> D = Mat3D <_Tp>::diag(eVals.getSqrt()); // pierwiastek z macierzy diagonalnej to pierwiastek z wektora diagonalnego
    return eVects*D*eVects.getInv();
}

template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getRotedX(const _Tp theta) const // obliczenie zrotowanej macierzy wzgledem osi X o kat theta
{
    if (theta == (roboLib::pi / 2))
    {
        return (*this)*Mat3D <_Tp> // mnozenie aktualnej macierzy razy macierz rotacji odpowiedniej dla zadanej osi i kata theta
                (
                    1, 0, 0,
                    0, 0, 1,
                    0, -1, 0
                    );
    }
    if (theta == (-roboLib::pi / 2))
    {
        return (*this)*Mat3D <_Tp> // mnozenie aktualnej macierzy razy macierz rtoacji odpowiedniej dla zadanej osi i kata theta
                (
                    1, 0, 0,
                    0, 0, -1,
                    0, 1, 0
                    );
    }
    return (*this)*Mat3D <_Tp> // mnozenie aktualnej macierzy razy macierz rtoacji odpowiedniej dla zadanej osi i kata theta
            (
                1, 0, 0,
                0, std::cos(theta), std::sin(theta),
                0, -std::sin(theta), std::cos(theta)
                );
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getRotedY(const _Tp theta) const // obliczenie zrotowanej macierzy wzgledem osi Y o kat theta
{
    return (*this)*Mat3D <_Tp> // mnozenie aktualnej macierzy razy macierz rtoacji odpowiedniej dla zadanej osi i kata theta
            (
                std::cos(theta), 0, -std::sin(theta),
                0, 1, 0,
                std::sin(theta), 0, std::cos(theta)
                );
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::getRotedZ(const _Tp theta) const // obliczenie zrotowanej macierzy wzgledem osi Z o kat theta
{
    return (*this)*Mat3D <_Tp> // mnozenie aktualnej macierzy razy macierz rtoacji odpowiedniej dla zadanej osi i kata theta
            (
                std::cos(theta), std::sin(theta), 0,
                -std::sin(theta), std::cos(theta), 0,
                0, 0, 1
                );
}

template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::getRoted(const Vect3D <_Type> &rpy) const // obliczenie zrotowanej macierzy o wektor katow PRY
{
    return Mat3D<_Tp>
            (
                );
}

template <class _Tp>
Vect3D <_Tp> Mat3D <_Tp>::getEulerAngles(void) const // obliczenie wektora katow PRY
{
    return Vect3D <_Tp>(
                std::atan2(mat[5], mat[8]),
            -std::asin(mat[2]),
            std::atan2(mat[1], mat[0])
            );
}
template <class _Tp>
Vect4D <_Tp> Mat3D <_Tp>::getQuaternion(void) const // obliczenie wektora katow PRY
{
    const _Tp tr = this->getTrace();
    Vect4D<_Tp> q;
    if (tr > 0)
    {
        _Tp S = sqrt(tr + 1) * 2; // S=4*qw
        q.d = S / 4;
        q.a = (mat[5] - mat[7]) / S;
        q.b = (mat[6] - mat[2]) / S;
        q.c = (mat[1] - mat[3]) / S;
    }
    else if (mat[0] > mat[4] && mat[0] > mat[8])
    {
        _Tp S = sqrt(1 + mat[0] - mat[4] - mat[8]) * 2; // S=4*qx
        q.d = (mat[5] - mat[7]) / S;
        q.a = S / 4;
        q.b = (mat[3] + mat[1]) / S;
        q.c = (mat[6] + mat[2]) / S;
    }
    else if (mat[4] > mat[8])
    {
        _Tp S = sqrt(1 + mat[4] - mat[0] - mat[8]) * 2; // S=4*qy
        q.d = (mat[6] - mat[2]) / S;
        q.a = (mat[3] + mat[1]) / S;
        q.b = S / 4;
        q.c = (mat[7] + mat[5]) / S;
    }
    else
    {
        _Tp S = sqrt(1 + mat[8] - mat[0] - mat[4]) * 2; // S=4*qz
        q.d = (mat[1] - mat[3]) / S;
        q.a = (mat[6] + mat[2]) / S;
        q.b = (mat[7] + mat[5]) / S;
        q.c = S / 4;
    }
    return q;
}
template <class _Tp>
Mat2D <_Tp> Mat3D <_Tp>::getFlatDcm(void) const // obliczanie macierzy rotacji dla ukladu plaskiego (osie XY, tylko yaw)
{
    const _Tp yaw = std::atan2(mat[1], mat[0]);
    const _Tp S = std::sin(yaw);
    const _Tp C = std::cos(yaw);
    return Mat2D <_Tp>(
                C, S,
                -S, C
                );
}

// data validation
template <class _Tp>
bool Mat3D <_Tp>::isNormal(void) const // check if data is correct
{
    return std::isnormal(this->mat[0]) &&
            std::isnormal(this->mat[1]) &&
            std::isnormal(this->mat[2]) &&
            std::isnormal(this->mat[3]) &&
            std::isnormal(this->mat[4]) &&
            std::isnormal(this->mat[5]) &&
            std::isnormal(this->mat[6]) &&
            std::isnormal(this->mat[7]) &&
            std::isnormal(this->mat[8]);
}

// metody statyczne
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::eye(void) // macierz jednostkowa
{
    return Mat3D <_Tp>();
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::ones(void) // macierz jedynkowa
{
    return Mat3D <_Tp>(1, 1, 1, 1, 1, 1, 1, 1, 1);
}
template <class _Tp>
Mat3D <_Tp> Mat3D <_Tp>::zeros(void) // macierz zerowa
{
    return Mat4D <_Tp>(0, 0, 0, 0, 0, 0, 0, 0, 0);
}
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::diag(const _Type c) // macierz diagonalna ze stalej
{
    return Mat3D <_Tp>
            (
                _Tp(c), 0, 0,
                0, _Tp(c), 0,
                0, 0, _Tp(c)
                );
}
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::diag(const Vect3D<_Type>& v) // macierz diagonalna z wekora
{
    return Mat3D <_Tp>
            (
                _Tp(v.x), 0, 0,
                0, _Tp(v.y), 0,
                0, 0, _Tp(v.z)
                );
}
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::vectAsRows(const Vect3D <_Type> &v1, const Vect3D <_Type> &v2, const Vect3D <_Type> &v3)
{
    return Mat3D <_Tp>
            (
                _Tp(v1.x), _Tp(v1.y), _Tp(v1.z),
                _Tp(v2.x), _Tp(v2.y), _Tp(v2.z),
                _Tp(v3.x), _Tp(v3.y), _Tp(v3.z)
                );
}
template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::vectAsCols(const Vect3D <_Type> &v1, const Vect3D <_Type> &v2, const Vect3D <_Type> &v3)
{
    return Mat3D <_Tp>
            (
                _Tp(v1.x), _Tp(v2.x), _Tp(v3.x),
                _Tp(v1.y), _Tp(v2.y), _Tp(v3.y),
                _Tp(v1.z), _Tp(v2.z), _Tp(v3.z)
                );
}

template <class _Tp> template <typename _Type>
Mat3D <_Tp> Mat3D <_Tp>::dcmFromEuler(const Vect3D<_Type> &rpy) // macierz kosinusow kierunkowych utworzona z wektora katow Eulera
{
    const _Tp sinR = std::sin(rpy.x);
    const _Tp cosR = std::cos(rpy.x);
    const _Tp sinP = std::sin(rpy.y);
    const _Tp cosP = std::cos(rpy.y);
    const _Tp sinY = std::sin(rpy.z);
    const _Tp cosY = std::cos(rpy.z);
    return Mat3D <_Tp>
            (
                cosP * cosY,
                cosP * sinY,
                -sinP,
                sinR * sinP * cosY - cosR * sinY,
                sinR * sinP * sinY + cosR * cosY,
                sinR * cosP,
                cosR * sinP * cosY + sinR * sinY,
                cosR * sinP * sinY - sinR * cosY,
                cosR * cosP
                );
}


// ================================== Mat4D ====================================
// konstruktory
template <class _Tp>
inline Mat4D <_Tp>::Mat4D(void) // defoultowy (macierz jednostkowa)
{
    mat[0] = 1;		mat[1] = 0;		mat[2] = 0;		mat[3] = 0;
    mat[4] = 0;		mat[5] = 1;		mat[6] = 0;		mat[7] = 0;
    mat[8] = 0;		mat[9] = 0;		mat[10] = 1;	mat[11] = 0;
    mat[12] = 0;	mat[13] = 0;	mat[14] = 0;	mat[15] = 1;
}
template <class _Tp>
inline Mat4D <_Tp>::Mat4D(const _Tp m0, const _Tp m1, const _Tp m2, const _Tp m3, const _Tp m4, const _Tp m5, const _Tp m6, const _Tp m7, const _Tp m8, const _Tp m9, const _Tp m10, const _Tp m11, const _Tp m12, const _Tp m13, const _Tp m14, const _Tp m15)
{
    mat[0] = m0;		mat[1] = m1;		mat[2] = m2;		mat[3] = m3;
    mat[4] = m4;		mat[5] = m5;		mat[6] = m6;		mat[7] = m7;
    mat[8] = m8;		mat[9] = m9;		mat[10] = m10;		mat[11] = m11;
    mat[12] = m12;		mat[13] = m13;		mat[14] = m14;		mat[15] = m15;
}
template <class _Tp> template <typename _Type>
inline Mat4D <_Tp>::Mat4D(const Mat4D <_Type> &m)
{
    mat[0] = _Tp(m.mat[0]);		mat[1] = _Tp(m.mat[1]);		mat[2] = _Tp(m.mat[2]);		mat[3] = _Tp(m.mat[3]);
    mat[4] = _Tp(m.mat[4]);		mat[5] = _Tp(m.mat[5]);		mat[6] = _Tp(m.mat[6]);		mat[7] = _Tp(m.mat[7]);
    mat[8] = _Tp(m.mat[8]);		mat[9] = _Tp(m.mat[9]);		mat[10] = _Tp(m.mat[10]);	mat[11] = _Tp(m.mat[11]);
    mat[12] = _Tp(m.mat[12]);	mat[13] = _Tp(m.mat[13]);	mat[14] = _Tp(m.mat[14]);	mat[15] = _Tp(m.mat[15]);
}

template <class _Tp>
Mat4D <_Tp>::Mat4D(const Matrix<_Tp>& matrix)
{
    if (matrix.size() >= size())
    {
        for (unsigned i = 0; i < size(); i++)
        {
            for (unsigned j = 0; j < size(); j++)
            {
                mat[i * size() + j] = matrix(i + 1, j + 1);
            }
        }
    }
    else
    {
        *this = Mat4D(); // exeption should be thrown
    }
}

template <class _Tp>
inline _Tp& Mat4D <_Tp>::operator () (const unsigned row, const unsigned column)
{
    if (!((row - 1 >= 4) || (column - 1 >= 4) || (row - 1 < 0) || (column - 1 < 0)))
    {
        return mat[(row - 1) * 4 + (column - 1)];
    }
    else return mat[0]; // exeption should be thrown
}

template <class _Tp>
inline const _Tp& Mat4D <_Tp>::operator () (const unsigned row, const unsigned column) const // tylko odczyt
{
    if (!((row - 1 >= 4) || (column - 1 >= 4) || (row - 1 < 0) || (column - 1 < 0)))
    {
        return mat[(row - 1) * 4 + (column - 1)];
    }
    else return mat[0]; // exeption should be thrown
}

template <class _Tp>
inline unsigned Mat4D <_Tp>::size() const // rozmiar wektora
{
    return 4;
}

// operatory logiczne
template <class _Tp> template <typename _Type>
inline bool Mat4D <_Tp>::operator == (const Mat4D <_Type> &m) const // rownosc
{
    return
            mat[0] == m.mat[0] && mat[1] == m.mat[1] && mat[2] == m.mat[2] && mat[3] == m.mat[3] &&
            mat[4] == m.mat[4] && mat[5] == m.mat[5] && mat[6] == m.mat[6] && mat[7] == m.mat[7] &&
            mat[8] == m.mat[8] && mat[9] == m.mat[9] && mat[10] == m.mat[10] && mat[11] == m.mat[11] &&
            mat[12] == m.mat[12] && mat[13] == m.mat[13] && mat[14] == m.mat[14] && mat[15] == m.mat[15];
}
template <class _Tp> template <typename _Type>
inline bool Mat4D <_Tp>::operator != (const Mat4D <_Type> &m) const // nierownosc
{
    return
            mat[0] != m.mat[0] || mat[1] != m.mat[1] || mat[2] != m.mat[2] || mat[3] != m.mat[3] ||
            mat[4] != m.mat[4] || mat[5] != m.mat[5] || mat[6] != m.mat[6] || mat[7] != m.mat[7] ||
            mat[8] != m.mat[8] || mat[9] != m.mat[9] || mat[10] != m.mat[10] || mat[11] != m.mat[11] ||
            mat[12] != m.mat[12] || mat[13] != m.mat[13] || mat[14] != m.mat[14] || mat[15] != m.mat[15];
}

// operacje analityczne
template <class _Tp> template <typename _Type>
Mat4D <_Tp> Mat4D <_Tp>::operator + (const _Type a) const // dodawanie stalej
{
    return Mat4D <_Tp>
            (
                a + mat[0], a + mat[1], a + mat[2], a + mat[3],
            a + mat[4], a + mat[5], a + mat[6], a + mat[7],
            a + mat[8], a + mat[9], a + mat[10], a + mat[11],
            a + mat[12], a + mat[13], a + mat[14], a + mat[15]
            );
}
template <class _Tp> template <typename _Type>
Mat4D <_Tp> Mat4D <_Tp>::operator + (const Mat4D <_Type> &m) const // dodawanie macierzy
{
    return Mat4D <_Tp>
            (
                mat[0] + m.mat[0], mat[1] + m.mat[1], mat[2] + m.mat[2], mat[3] + m.mat[3],
            mat[4] + m.mat[4], mat[5] + m.mat[5], mat[6] + m.mat[6], mat[7] + m.mat[7],
            mat[8] + m.mat[8], mat[9] + m.mat[9], mat[10] + m.mat[10], mat[11] + m.mat[11],
            mat[12] + m.mat[12], mat[13] + m.mat[13], mat[14] + m.mat[14], mat[15] + m.mat[15]
            );
}

template <class _Tp> template <typename _Type>
Mat4D <_Tp> Mat4D <_Tp>::operator - (const _Type a) const // odejmowanie stalej
{
    return Mat4D <_Tp>
            (
                mat[0] - a, mat[1] - a, mat[2] - a, mat[3] - a,
            mat[4] - a, mat[5] - a, mat[6] - a, mat[7] - a,
            mat[8] - a, mat[9] - a, mat[10] - a, mat[11] - a,
            mat[12] - a, mat[13] - a, mat[14] - a, mat[15] - a
            );
}
template <class _Tp> template <typename _Type>
Mat4D <_Tp> Mat4D <_Tp>::operator - (const Mat4D <_Type> &m) const // odejmowanie macierzy
{
    return Mat4D <_Tp>
            (
                mat[0] - m.mat[0], mat[1] - m.mat[1], mat[2] - m.mat[2], mat[3] - m.mat[3],
            mat[4] - m.mat[4], mat[5] - m.mat[5], mat[6] - m.mat[6], mat[7] - m.mat[7],
            mat[8] - m.mat[8], mat[9] - m.mat[9], mat[10] - m.mat[10], mat[11] - m.mat[11],
            mat[12] - m.mat[12], mat[13] - m.mat[13], mat[14] - m.mat[14], mat[15] - m.mat[15]
            );
}

template <class _Tp> template <typename _Type>
Mat4D <_Tp> Mat4D <_Tp>::operator * (const _Type a) const // mnozenie razy stala
{
    return Mat4D <_Tp>
            (
                _Tp(a*mat[0]), _Tp(a*mat[1]), _Tp(a*mat[2]), _Tp(a*mat[3]),
            _Tp(a*mat[4]), _Tp(a*mat[5]), _Tp(a*mat[6]), _Tp(a*mat[7]),
            _Tp(a*mat[8]), _Tp(a*mat[9]), _Tp(a*mat[10]), _Tp(a*mat[11]),
            _Tp(a*mat[12]), _Tp(a*mat[13]), _Tp(a*mat[14]), _Tp(a*mat[15])
            );
}
template <class _Tp> template <typename _Type>
Vect4D <_Tp> Mat4D <_Tp>::operator * (const Vect4D <_Type> &v) const // mnozenie razy wektor
{
    return Vect4D <_Tp>
            (
                _Tp(mat[0] * v.a + mat[1] * v.b + mat[2] * v.c + mat[3] * v.d),
            _Tp(mat[4] * v.a + mat[5] * v.b + mat[6] * v.c + mat[7] * v.d),
            _Tp(mat[8] * v.a + mat[9] * v.b + mat[10] * v.c + mat[11] * v.d),
            _Tp(mat[12] * v.a + mat[13] * v.b + mat[14] * v.c + mat[15] * v.d)
            );
}
template <class _Tp> template <typename _Type>
Mat4D <_Tp> Mat4D <_Tp>::operator * (const Mat4D <_Type> &m) const // mnozenie razy macierz
{
    return Mat4D <_Tp>
            (
                mat[0] * m.mat[0] + mat[1] * m.mat[4] + mat[2] * m.mat[8] + mat[3] * m.mat[12],
            mat[0] * m.mat[1] + mat[1] * m.mat[5] + mat[2] * m.mat[9] + mat[3] * m.mat[13],
            mat[0] * m.mat[2] + mat[1] * m.mat[6] + mat[2] * m.mat[10] + mat[3] * m.mat[14],
            mat[0] * m.mat[3] + mat[1] * m.mat[7] + mat[2] * m.mat[11] + mat[3] * m.mat[15],
            mat[4] * m.mat[0] + mat[5] * m.mat[4] + mat[6] * m.mat[8] + mat[7] * m.mat[12],
            mat[4] * m.mat[1] + mat[5] * m.mat[5] + mat[6] * m.mat[9] + mat[7] * m.mat[13],
            mat[4] * m.mat[2] + mat[5] * m.mat[6] + mat[6] * m.mat[10] + mat[7] * m.mat[14],
            mat[4] * m.mat[3] + mat[5] * m.mat[7] + mat[6] * m.mat[11] + mat[7] * m.mat[15],
            mat[8] * m.mat[0] + mat[9] * m.mat[4] + mat[10] * m.mat[8] + mat[11] * m.mat[12],
            mat[8] * m.mat[1] + mat[9] * m.mat[5] + mat[10] * m.mat[9] + mat[11] * m.mat[13],
            mat[8] * m.mat[2] + mat[9] * m.mat[6] + mat[10] * m.mat[10] + mat[11] * m.mat[14],
            mat[8] * m.mat[3] + mat[9] * m.mat[7] + mat[10] * m.mat[11] + mat[11] * m.mat[15],
            mat[12] * m.mat[0] + mat[13] * m.mat[4] + mat[14] * m.mat[8] + mat[15] * m.mat[12],
            mat[12] * m.mat[1] + mat[13] * m.mat[5] + mat[14] * m.mat[9] + mat[15] * m.mat[13],
            mat[12] * m.mat[2] + mat[13] * m.mat[6] + mat[14] * m.mat[10] + mat[15] * m.mat[14],
            mat[12] * m.mat[3] + mat[13] * m.mat[7] + mat[14] * m.mat[11] + mat[15] * m.mat[15]
            );
}

template <class _Tp> template <typename _Type>
Vect4D <_Tp> Mat4D <_Tp>::transMul(const Vect4D <_Type> &v) const // mnozenie transponowanej macierzy razy wektor
{
    return Vect4D <_Tp>
            (
                v.a*mat[0] + v.b*mat[4] + v.c*mat[8] + v.d*mat[12],
            v.a*mat[1] + v.b*mat[5] + v.c*mat[9] + v.d*mat[13],
            v.a*mat[2] + v.b*mat[6] + v.c*mat[10] + v.d*mat[14],
            v.a*mat[3] + v.b*mat[7] + v.c*mat[11] + v.d*mat[15]
            );
}
template <class _Tp> template <typename _Type>
Mat4D <_Tp> Mat4D <_Tp>::transMul(const Mat4D <_Type> &m) const // mnozenie transponowanej macierzy razy macierz
{
    return Mat4D <_Tp>
            (
                m.mat[0] * mat[0] + m.mat[4] * mat[4] + m.mat[8] * mat[8] + m.mat[12] * mat[12],
            m.mat[1] * mat[0] + m.mat[5] * mat[4] + m.mat[9] * mat[8] + m.mat[13] * mat[12],
            m.mat[2] * mat[0] + m.mat[6] * mat[4] + m.mat[10] * mat[8] + m.mat[14] * mat[12],
            m.mat[3] * mat[0] + m.mat[7] * mat[4] + m.mat[11] * mat[8] + m.mat[15] * mat[12],
            m.mat[0] * mat[1] + m.mat[4] * mat[5] + m.mat[8] * mat[9] + m.mat[12] * mat[13],
            m.mat[1] * mat[1] + m.mat[5] * mat[5] + m.mat[9] * mat[9] + m.mat[13] * mat[13],
            m.mat[2] * mat[1] + m.mat[6] * mat[5] + m.mat[10] * mat[9] + m.mat[14] * mat[13],
            m.mat[3] * mat[1] + m.mat[7] * mat[5] + m.mat[11] * mat[9] + m.mat[15] * mat[13],
            m.mat[0] * mat[2] + m.mat[4] * mat[6] + m.mat[8] * mat[10] + m.mat[12] * mat[14],
            m.mat[1] * mat[2] + m.mat[5] * mat[6] + m.mat[9] * mat[10] + m.mat[13] * mat[14],
            m.mat[2] * mat[2] + m.mat[6] * mat[6] + m.mat[10] * mat[10] + m.mat[14] * mat[14],
            m.mat[3] * mat[2] + m.mat[7] * mat[6] + m.mat[11] * mat[10] + m.mat[15] * mat[14],
            m.mat[0] * mat[3] + m.mat[4] * mat[7] + m.mat[8] * mat[11] + m.mat[12] * mat[15],
            m.mat[1] * mat[3] + m.mat[5] * mat[7] + m.mat[9] * mat[11] + m.mat[13] * mat[15],
            m.mat[2] * mat[3] + m.mat[6] * mat[7] + m.mat[10] * mat[11] + m.mat[14] * mat[15],
            m.mat[3] * mat[3] + m.mat[7] * mat[7] + m.mat[11] * mat[11] + m.mat[15] * mat[15]
            );
}

template <class _Tp> template <typename _Type>
inline Mat4D <_Tp> Mat4D <_Tp>::operator / (const _Type a) const // dzielenie
{
    return Mat4D <_Tp>
            (
                mat[0] / a, mat[1] / a, mat[2] / a, mat[3] / a,
            mat[4] / a, mat[5] / a, mat[6] / a, mat[7] / a,
            mat[8] / a, mat[9] / a, mat[10] / a, mat[11] / a,
            mat[12] / a, mat[13] / a, mat[14] / a, mat[15] / a
            );
}

template <class _Tp>
const _Tp* Mat4D <_Tp>::begin(void) const // wskaznik na poczatek tablicy macierzy
{
    return mat;
}
template <class _Tp>
const _Tp* Mat4D <_Tp>::end(void) const // wskaznik na koniec tablicy macierzy
{
    return mat + 16;
}

template <class _Tp>
_Tp Mat4D <_Tp>::getDet(void) const // wyznacznik macierzy
{
    return mat[0] * mat[5] * mat[10] * mat[15] - mat[0] * mat[5] * mat[11] * mat[14] - mat[0] * mat[6] * mat[9] * mat[15] + mat[0] * mat[6] * mat[11] * mat[13] + mat[0] * mat[7] * mat[9] * mat[14] - mat[0] * mat[7] * mat[10] * mat[13] - mat[1] * mat[4] * mat[10] * mat[15] + mat[1] * mat[4] * mat[11] * mat[14] + mat[1] * mat[6] * mat[8] * mat[15] - mat[1] * mat[6] * mat[11] * mat[12] - mat[1] * mat[7] * mat[8] * mat[14] + mat[1] * mat[7] * mat[10] * mat[12] + mat[2] * mat[4] * mat[9] * mat[15] - mat[2] * mat[4] * mat[11] * mat[13] - mat[2] * mat[5] * mat[8] * mat[15] + mat[2] * mat[5] * mat[11] * mat[12] + mat[2] * mat[7] * mat[8] * mat[13] - mat[2] * mat[7] * mat[9] * mat[12] - mat[3] * mat[4] * mat[9] * mat[14] + mat[3] * mat[4] * mat[10] * mat[13] + mat[3] * mat[5] * mat[8] * mat[14] - mat[3] * mat[5] * mat[10] * mat[12] - mat[3] * mat[6] * mat[8] * mat[13] + mat[3] * mat[6] * mat[9] * mat[12];
}
template <class _Tp>
_Tp Mat4D <_Tp>::getTrace(void) const // slad macierzy
{
    return mat[0] + mat[5] + mat[10] + mat[15];
}

template <class _Tp>
Mat4D <_Tp> Mat4D <_Tp>::getTrans(void) const // transpozycja macierzy
{
    return Mat4D <_Tp>
            (
                mat[0], mat[4], mat[8], mat[12],
            mat[1], mat[5], mat[9], mat[13],
            mat[2], mat[6], mat[10], mat[14],
            mat[3], mat[7], mat[11], mat[15]
            );
}
template <class _Tp>
Mat4D <_Tp> Mat4D <_Tp>::getInv(void) const // odwracanie macierzy
{
    const _Tp d = this->getDet();
    return Mat4D <_Tp>
            (
                (mat[5] * mat[10] * mat[15] - mat[5] * mat[11] * mat[14] - mat[6] * mat[9] * mat[15] + mat[6] * mat[11] * mat[13] + mat[7] * mat[9] * mat[14] - mat[7] * mat[10] * mat[13]) / d,
            -(mat[1] * mat[10] * mat[15] - mat[1] * mat[11] * mat[14] - mat[2] * mat[9] * mat[15] + mat[2] * mat[11] * mat[13] + mat[3] * mat[9] * mat[14] - mat[3] * mat[10] * mat[13]) / d,
            (mat[1] * mat[6] * mat[15] - mat[1] * mat[7] * mat[14] - mat[2] * mat[5] * mat[15] + mat[2] * mat[7] * mat[13] + mat[3] * mat[5] * mat[14] - mat[3] * mat[6] * mat[13]) / d,
            -(mat[1] * mat[6] * mat[11] - mat[1] * mat[7] * mat[10] - mat[2] * mat[5] * mat[11] + mat[2] * mat[7] * mat[9] + mat[3] * mat[5] * mat[10] - mat[3] * mat[6] * mat[9]) / d,
            -(mat[4] * mat[10] * mat[15] - mat[4] * mat[11] * mat[14] - mat[6] * mat[8] * mat[15] + mat[6] * mat[11] * mat[12] + mat[7] * mat[8] * mat[14] - mat[7] * mat[10] * mat[12]) / d,
            (mat[0] * mat[10] * mat[15] - mat[0] * mat[11] * mat[14] - mat[2] * mat[8] * mat[15] + mat[2] * mat[11] * mat[12] + mat[3] * mat[8] * mat[14] - mat[3] * mat[10] * mat[12]) / d,
            -(mat[0] * mat[6] * mat[15] - mat[0] * mat[7] * mat[14] - mat[2] * mat[4] * mat[15] + mat[2] * mat[7] * mat[12] + mat[3] * mat[4] * mat[14] - mat[3] * mat[6] * mat[12]) / d,
            (mat[0] * mat[6] * mat[11] - mat[0] * mat[7] * mat[10] - mat[2] * mat[4] * mat[11] + mat[2] * mat[7] * mat[8] + mat[3] * mat[4] * mat[10] - mat[3] * mat[6] * mat[8]) / d,
            (mat[4] * mat[9] * mat[15] - mat[4] * mat[11] * mat[13] - mat[5] * mat[8] * mat[15] + mat[5] * mat[11] * mat[12] + mat[7] * mat[8] * mat[13] - mat[7] * mat[9] * mat[12]) / d,
            -(mat[0] * mat[9] * mat[15] - mat[0] * mat[11] * mat[13] - mat[1] * mat[8] * mat[15] + mat[1] * mat[11] * mat[12] + mat[3] * mat[8] * mat[13] - mat[3] * mat[9] * mat[12]) / d,
            (mat[0] * mat[5] * mat[15] - mat[0] * mat[7] * mat[13] - mat[1] * mat[4] * mat[15] + mat[1] * mat[7] * mat[12] + mat[3] * mat[4] * mat[13] - mat[3] * mat[5] * mat[12]) / d,
            -(mat[0] * mat[5] * mat[11] - mat[0] * mat[7] * mat[9] - mat[1] * mat[4] * mat[11] + mat[1] * mat[7] * mat[8] + mat[3] * mat[4] * mat[9] - mat[3] * mat[5] * mat[8]) / d,
            -(mat[4] * mat[9] * mat[14] - mat[4] * mat[10] * mat[13] - mat[5] * mat[8] * mat[14] + mat[5] * mat[10] * mat[12] + mat[6] * mat[8] * mat[13] - mat[6] * mat[9] * mat[12]) / d,
            (mat[0] * mat[9] * mat[14] - mat[0] * mat[10] * mat[13] - mat[1] * mat[8] * mat[14] + mat[1] * mat[10] * mat[12] + mat[2] * mat[8] * mat[13] - mat[2] * mat[9] * mat[12]) / d,
            -(mat[0] * mat[5] * mat[14] - mat[0] * mat[6] * mat[13] - mat[1] * mat[4] * mat[14] + mat[1] * mat[6] * mat[12] + mat[2] * mat[4] * mat[13] - mat[2] * mat[5] * mat[12]) / d,
            (mat[0] * mat[5] * mat[10] - mat[0] * mat[6] * mat[9] - mat[1] * mat[4] * mat[10] + mat[1] * mat[6] * mat[8] + mat[2] * mat[4] * mat[9] - mat[2] * mat[5] * mat[8]) / d
            );
}


// metody statyczne
template <class _Tp>
Mat4D <_Tp> Mat4D <_Tp>::eye(void) // macierz jednostkowa
{
    return Mat4D <_Tp>();
}
template <class _Tp>
Mat4D <_Tp> Mat4D <_Tp>::ones(void) // macierz jedynkowa
{
    return Mat4D <_Tp>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
}
template <class _Tp>
Mat4D <_Tp> Mat4D <_Tp>::zeros(void) // macierz zerowa
{
    return Mat4D <_Tp>(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}


// ================================== Vector ====================================
// konstruktory
template <class _Tp>
Vector <_Tp>::Vector(const Vector<_Tp>& v)
{
    dataSize = v.size();
    data = new _Tp[dataSize];
    for (unsigned i = 0; i < dataSize; i++)
    {
        data[i] = v(i + 1);
    }
}
template <class _Tp>
Vector <_Tp>::Vector(const unsigned _dataSize)
{
    if (_dataSize <= 0) dataSize = 1;
    else dataSize = _dataSize;
    data = new _Tp[dataSize];
}
template <class _Tp>
Vector <_Tp>::Vector(const Vect2D<_Tp>& v)
{
    dataSize = v.size();
    data = new _Tp[dataSize];
    data[0] = v.x;
    data[1] = v.y;
}
template <class _Tp>
Vector <_Tp>::Vector(const Vect3D<_Tp>& v)
{
    dataSize = v.size();
    data = new _Tp[dataSize];
    data[0] = v.x;
    data[1] = v.y;
    data[2] = v.z;
}
template <class _Tp>
Vector <_Tp>::Vector(const Vect4D<_Tp>& v)
{
    dataSize = v.size();
    data = new _Tp[dataSize];
    data[0] = v.a;
    data[1] = v.b;
    data[2] = v.c;
    data[4] = v.d;
}

template <class _Tp>
Vector <_Tp>::~Vector(void)
{
    delete[] data;
}

// operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
template <class _Tp>
_Tp& Vector <_Tp>::operator () (const unsigned row)
{
    if (!((row - 1 >= dataSize) || (row - 1 < 0)))
    {
        return data[row - 1];
    }
    else return data[0]; // exeption should be thrown
}
template <class _Tp>
_Tp Vector <_Tp>::operator () (const unsigned row) const // tylko odczyt
{
    if (!((row - 1 >= dataSize) || (row - 1 < 0)))
    {
        return data[row - 1];
    }
    else return data[0]; // exeption should be thrown, out of range
}

// operatory arytmetyczne
template <class _Tp> template <typename _Type>
Vector <_Tp> Vector <_Tp>::operator + (const _Type a) const // dodawanie stalej
{
    Vector <_Tp> out(dataSize);
    for (unsigned i = 0; i < dataSize; i++)
    {
        out(i + 1) = data[i] + a;
    }
    return out;
}
template <class _Tp> template <typename _Type>
Vector <_Tp> Vector <_Tp>::operator + (const Vector <_Type>& v) const // dodawanie wektora
{
    if (dataSize == v.size())
    {
        Vector <_Tp> out(dataSize);
        for (unsigned i = 0; i < dataSize; i++)
        {
            out(i + 1) = data[i] + _Tp(v(i + 1));
        }
        return out;
    }
    else
    {
        return Vector <_Tp>(1);// exeption should be thrown, bad size
    }
}

template <class _Tp>
void Vector <_Tp>::mul(const Matrix<_Tp>& left, const Vector<_Tp>& right)
{
    for (unsigned i = 0; i < dataSize; i++)
    {
        _Tp sum = 0.0;
        for (unsigned j = 0; j < dataSize; j++)
        {
            sum += left(i + 1, j + 1) * right(j + 1);
        }
        data[i] = sum;
    }
}
template <class _Tp>
void Vector <_Tp>::copyVector(const Vector&  source)
{
    for (unsigned i = 0; i < dataSize; i++)
    {
        data[i] = source(i + 1);
    }
}
template <class _Tp>
unsigned Vector <_Tp>::size() const
{
    return dataSize;
}

template <class _Tp>
Vector<_Tp> Vector <_Tp>::zeros(const unsigned dataSize)
{
    Vector<_Tp> v(dataSize);
    for (unsigned i = 0; i < dataSize; i++)
    {
        v(i + 1) = 0;
    }
    return v;
}


// ================================== Matrix ====================================
// konstruktory
template <class _Tp>
Matrix <_Tp>::Matrix(const Matrix<_Tp>& m)
{
    dataSize = m.size();
    data = new _Tp[dataSize*dataSize];
    for (unsigned i = 0; i < dataSize; i++)
        for (unsigned j = 0; j < dataSize; j++)
        {
            data[i*dataSize + j] = m(i + 1, j + 1);
        }
}
template <class _Tp>
Matrix <_Tp>::Matrix(const unsigned _dataSize)
{
    if (_dataSize <= 0) dataSize = 1;
    else dataSize = _dataSize;
    data = new _Tp[dataSize*dataSize];
}
template <class _Tp>
Matrix <_Tp>::Matrix(const Mat2D<_Tp>& m)
{
    dataSize = m.size();
    data = new _Tp[dataSize*dataSize];
    for (unsigned i = 0; i < dataSize; i++)
    {
        for (unsigned j = 0; j < dataSize; j++)
        {
            data[i * dataSize + j] = m(i + 1, j + 1);
        }
    }
}

template <class _Tp>
Matrix <_Tp>::Matrix(const Mat3D<_Tp>& m)
{
    dataSize = m.size();
    data = new _Tp[dataSize*dataSize];
    for (unsigned i = 0; i < dataSize; i++)
    {
        for (unsigned j = 0; j < dataSize; j++)
        {
            data[i * dataSize + j] = m(i + 1, j + 1);
        }
    }
}

template <class _Tp>
Matrix <_Tp>::Matrix(const Mat4D<_Tp>& m)
{
    dataSize = m.size();
    data = new _Tp[dataSize*dataSize];
    for (unsigned i = 0; i < dataSize; i++)
    {
        for (unsigned j = 0; j < dataSize; j++)
        {
            data[i * dataSize + j] = m(i + 1, j + 1);
        }
    }
}

template <class _Tp>
Matrix <_Tp>::~Matrix(void)
{
    delete[] data;
}


// operatory dostepu (UWAGA! elementy numerowane w stylu Matlaba - od 1!)
template <class _Tp>
_Tp& Matrix <_Tp>::operator () (const unsigned row, const unsigned column)
{
    if (!((row - 1 >= dataSize) || (column - 1 >= dataSize) || (row - 1 < 0) || (column - 1 < 0)))
    {
        return data[(row - 1) * dataSize + (column - 1)];
    }
    else return data[0]; // exeption should be thrown
}
template <class _Tp>
_Tp Matrix <_Tp>::operator () (const unsigned row, const unsigned column) const
{
    if (!((row - 1 >= dataSize) || (column - 1 >= dataSize) || (row - 1 < 0) || (column - 1 < 0)))
    {
        return data[(row - 1) * dataSize + (column - 1)];
    }
    else return data[0]; // exeption should be thrown
}


template <class _Tp>
void Matrix <_Tp>::mul(const Matrix& left, const Matrix& right)
{
    for (int i = 0; i < dataSize; i++)
        for (int j = 0; j < dataSize; j++)
        {
            _Tp sum = 0.0;
            for (int c = 0; c < dataSize; c++)
            {
                sum += left(i + 1, c + 1) * right(c + 1, j + 1);
            }
            data[i*dataSize + j] = sum;
        }
}

template <class _Tp>
void Matrix <_Tp>::copyMatrix(const Matrix&  source)
{
    for (int i = 0; i < dataSize; i++)
        for (int j = 0; j < dataSize; j++)
        {
            data[i*dataSize + j] = source(i + 1, j + 1);
        }
}

template <class _Tp>
unsigned Matrix <_Tp>::size(void) const
{
    return dataSize;
}

template <class _Tp>
void Matrix <_Tp>::invert(void)
{
    for (unsigned i = 1; i < dataSize; i++) data[i] /= data[0]; // normalize row 0
    for (unsigned i = 1; i < dataSize; i++)
    {
        for (unsigned j = i; j < dataSize; j++) // do a column of L
        {
            _Tp sum = 0.0;
            for (unsigned k = 0; k < i; k++)
                sum += data[j*dataSize + k] * data[k*dataSize + i];
            data[j*dataSize + i] -= sum;
        }
        if (i == dataSize - 1) continue;
        for (unsigned j = i + 1; j < dataSize; j++) // do a row of U
        {
            _Tp sum = 0.0;
            for (unsigned k = 0; k < i; k++)
                sum += data[i*dataSize + k] * data[k*dataSize + j];
            data[i*dataSize + j] =
                    (data[i*dataSize + j] - sum) / data[i*dataSize + i];
        }
    }
    for (unsigned i = 0; i < dataSize; i++)  // invert L
        for (unsigned j = i; j < dataSize; j++)
        {
            _Tp x = 1.0;
            if (i != j)
            {
                x = 0.0;
                for (unsigned k = i; k < j; k++)
                    x -= data[j*dataSize + k] * data[k*dataSize + i];
            }
            data[j*dataSize + i] = x / data[j*dataSize + j];
        }
    for (unsigned i = 0; i < dataSize; i++)   // invert U
        for (unsigned j = i; j < dataSize; j++)
        {
            if (i == j) continue;
            _Tp sum = 0.0;
            for (unsigned k = i; k < j; k++)
                sum += data[k*dataSize + j] * ((i == k) ? 1.0 : data[i*dataSize + k]);
            data[i*dataSize + j] = -sum;
        }
    for (unsigned i = 0; i < dataSize; i++)   // final inversion
        for (unsigned j = 0; j < dataSize; j++)
        {
            _Tp sum = 0.0;
            for (unsigned k = ((i>j) ? i : j); k < dataSize; k++)
                sum += ((j == k) ? 1.0 : data[j*dataSize + k])*data[k*dataSize + i];
            data[j*dataSize + i] = sum;
        }
}

#endif // __ROBOLIB_CORE__
