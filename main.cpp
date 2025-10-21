#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class MatrizBase {
protected:
    int numeroFilas;
    int numeroColumnas;

public:
    MatrizBase(int filas, int columnas) : numeroFilas(filas), numeroColumnas(columnas) {}

    virtual ~MatrizBase() {}

    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;

    int obtenerFilas() const { return numeroFilas; }
    int obtenerColumnas() const { return numeroColumnas; }

    virtual T obtenerElemento(int i, int j) const = 0;
    virtual void establecerElemento(int i, int j, T valor) = 0;

    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return this->sumar(otra);
    }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T** arregloDatos;

    void asignarMemoria() {
        arregloDatos = new T*[this->numeroFilas];
        for(int indiceFila = 0; indiceFila < this->numeroFilas; indiceFila++) {
            arregloDatos[indiceFila] = new T[this->numeroColumnas];
            for(int indiceColumna = 0; indiceColumna < this->numeroColumnas; indiceColumna++) {
                arregloDatos[indiceFila][indiceColumna] = T();
            }
        }
    }

    void liberarMemoria() {
        if(arregloDatos != nullptr) {
            for(int indiceFila = 0; indiceFila < this->numeroFilas; indiceFila++) {
                delete[] arregloDatos[indiceFila];
            }
            delete[] arregloDatos;
            arregloDatos = nullptr;
        }
    }

    void copiarDatos(T** arregloOrigen) {
        for(int indiceFila = 0; indiceFila < this->numeroFilas; indiceFila++) {
            for(int indiceColumna = 0; indiceColumna < this->numeroColumnas; indiceColumna++) {
                arregloDatos[indiceFila][indiceColumna] = arregloOrigen[indiceFila][indiceColumna];
            }
        }
    }

public:
    MatrizDinamica(int cantidadFilas, int cantidadColumnas)
        : MatrizBase<T>(cantidadFilas, cantidadColumnas), arregloDatos(nullptr) {
        asignarMemoria();
    }

    ~MatrizDinamica() {
        liberarMemoria();
    }

    MatrizDinamica(const MatrizDinamica<T>& matrizOtra)
        : MatrizBase<T>(matrizOtra.numeroFilas, matrizOtra.numeroColumnas), arregloDatos(nullptr) {
        asignarMemoria();
        copiarDatos(matrizOtra.arregloDatos);
    }

    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& matrizOtra) {
        if(this != &matrizOtra) {
            liberarMemoria();

            this->numeroFilas = matrizOtra.numeroFilas;
            this->numeroColumnas = matrizOtra.numeroColumnas;

            asignarMemoria();
            copiarDatos(matrizOtra.arregloDatos);
        }
        return *this;
    }

    MatrizDinamica(MatrizDinamica<T>&& matrizOtra) noexcept
        : MatrizBase<T>(matrizOtra.numeroFilas, matrizOtra.numeroColumnas), arregloDatos(matrizOtra.arregloDatos) {
        matrizOtra.arregloDatos = nullptr;
        matrizOtra.numeroFilas = 0;
        matrizOtra.numeroColumnas = 0;
    }

    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& matrizOtra) noexcept {
        if(this != &matrizOtra) {
            liberarMemoria();

            this->numeroFilas = matrizOtra.numeroFilas;
            this->numeroColumnas = matrizOtra.numeroColumnas;
            arregloDatos = matrizOtra.arregloDatos;

            matrizOtra.arregloDatos = nullptr;
            matrizOtra.numeroFilas = 0;
            matrizOtra.numeroColumnas = 0;
        }
        return *this;
    }

    void cargarValores() override {
        std::cout << "Ingrese los valores de la matriz (" << this->numeroFilas
                  << "x" << this->numeroColumnas << "):\n";
        for(int indiceFila = 0; indiceFila < this->numeroFilas; indiceFila++) {
            for(int indiceColumna = 0; indiceColumna < this->numeroColumnas; indiceColumna++) {
                std::cout << "Elemento [" << indiceFila << "][" << indiceColumna << "]: ";
                std::cin >> arregloDatos[indiceFila][indiceColumna];
            }
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& matrizOtra) const override {
        if(this->numeroFilas != matrizOtra.obtenerFilas() || this->numeroColumnas != matrizOtra.obtenerColumnas()) {
            throw std::invalid_argument("Las dimensiones de las matrices no son compatibles para suma");
        }

        MatrizDinamica<T>* matrizResultado = new MatrizDinamica<T>(this->numeroFilas, this->numeroColumnas);

        for(int indiceFila = 0; indiceFila < this->numeroFilas; indiceFila++) {
            for(int indiceColumna = 0; indiceColumna < this->numeroColumnas; indiceColumna++) {
                T valorSuma = arregloDatos[indiceFila][indiceColumna] + matrizOtra.obtenerElemento(indiceFila, indiceColumna);
                matrizResultado->establecerElemento(indiceFila, indiceColumna, valorSuma);
            }
        }

        return matrizResultado;
    }

    void imprimir() const override {
        std::cout << std::fixed << std::setprecision(1);
        for(int indiceFila = 0; indiceFila < this->numeroFilas; indiceFila++) {
            std::cout << "| ";
            for(int indiceColumna = 0; indiceColumna < this->numeroColumnas; indiceColumna++) {
                std::cout << std::setw(5) << arregloDatos[indiceFila][indiceColumna] << " ";
            }
            std::cout << "|\n";
        }
    }

    T obtenerElemento(int indiceFila, int indiceColumna) const override {
        if(indiceFila >= 0 && indiceFila < this->numeroFilas && indiceColumna >= 0 && indiceColumna < this->numeroColumnas) {
            return arregloDatos[indiceFila][indiceColumna];
        }
        throw std::out_of_range("Indices fuera de rango");
    }

    void establecerElemento(int indiceFila, int indiceColumna, T valorElemento) override {
        if(indiceFila >= 0 && indiceFila < this->numeroFilas && indiceColumna >= 0 && indiceColumna < this->numeroColumnas) {
            arregloDatos[indiceFila][indiceColumna] = valorElemento;
        } else {
            throw std::out_of_range("Indices fuera de rango");
        }
    }
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T arregloDatos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for(int indiceFila = 0; indiceFila < M; indiceFila++) {
            for(int indiceColumna = 0; indiceColumna < N; indiceColumna++) {
                arregloDatos[indiceFila][indiceColumna] = T();
            }
        }
    }

    void cargarValores() override {
        std::cout << "Ingrese los valores de la matriz (" << M << "x" << N << "):\n";
        for(int indiceFila = 0; indiceFila < M; indiceFila++) {
            for(int indiceColumna = 0; indiceColumna < N; indiceColumna++) {
                std::cout << "Elemento [" << indiceFila << "][" << indiceColumna << "]: ";
                std::cin >> arregloDatos[indiceFila][indiceColumna];
            }
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& matrizOtra) const override {
        if(M != matrizOtra.obtenerFilas() || N != matrizOtra.obtenerColumnas()) {
            throw std::invalid_argument("Las dimensiones de las matrices no son compatibles para suma");
        }

        MatrizEstatica<T, M, N>* matrizResultado = new MatrizEstatica<T, M, N>();

        for(int indiceFila = 0; indiceFila < M; indiceFila++) {
            for(int indiceColumna = 0; indiceColumna < N; indiceColumna++) {
                T valorSuma = arregloDatos[indiceFila][indiceColumna] + matrizOtra.obtenerElemento(indiceFila, indiceColumna);
                matrizResultado->establecerElemento(indiceFila, indiceColumna, valorSuma);
            }
        }

        return matrizResultado;
    }

    void imprimir() const override {
        std::cout << std::fixed << std::setprecision(1);
        for(int indiceFila = 0; indiceFila < M; indiceFila++) {
            std::cout << "| ";
            for(int indiceColumna = 0; indiceColumna < N; indiceColumna++) {
                std::cout << std::setw(5) << arregloDatos[indiceFila][indiceColumna] << " ";
            }
            std::cout << "|\n";
        }
    }

    T obtenerElemento(int indiceFila, int indiceColumna) const override {
        if(indiceFila >= 0 && indiceFila < M && indiceColumna >= 0 && indiceColumna < N) {
            return arregloDatos[indiceFila][indiceColumna];
        }
        throw std::out_of_range("Indices fuera de rango");
    }

    void establecerElemento(int indiceFila, int indiceColumna, T valorElemento) override {
        if(indiceFila >= 0 && indiceFila < M && indiceColumna >= 0 && indiceColumna < N) {
            arregloDatos[indiceFila][indiceColumna] = valorElemento;
        } else {
            throw std::out_of_range("Indices fuera de rango");
        }
    }
};

int main() {
    std::cout << "Sistema Generico de Algebra Lineal\n\n";

    try {
        std::cout << "Demostracion de Genericidad (Tipo FLOAT)\n\n";

        std::cout << "1. Creacion de Matriz Dinamica A (3x2)\n";
        MatrizBase<float>* matrizA = new MatrizDinamica<float>(3, 2);
        std::cout << "Creando Matriz Dinamica A (3x2)\n";
        matrizA->establecerElemento(0, 0, 1.5f); matrizA->establecerElemento(0, 1, 2.0f);
        matrizA->establecerElemento(1, 0, 0.0f); matrizA->establecerElemento(1, 1, 1.0f);
        matrizA->establecerElemento(2, 0, 4.5f); matrizA->establecerElemento(2, 1, 3.0f);

        std::cout << "A\n";
        matrizA->imprimir();
        std::cout << "\n";

        std::cout << "2. Creacion de Matriz Estatica B (3x2)\n";
        MatrizBase<float>* matrizB = new MatrizEstatica<float, 3, 2>();
        std::cout << "Creando Matriz Estatica B (3x2)\n";
        matrizB->establecerElemento(0, 0, 0.5f); matrizB->establecerElemento(0, 1, 1.0f);
        matrizB->establecerElemento(1, 0, 2.0f); matrizB->establecerElemento(1, 1, 3.0f);
        matrizB->establecerElemento(2, 0, 1.0f); matrizB->establecerElemento(2, 1, 1.0f);

        std::cout << "B\n";
        matrizB->imprimir();
        std::cout << "\n";

        std::cout << "3. Operacion Polimorfica (Suma)\n";
        std::cout << "SUMANDO: Matriz C = A + B\n";
        std::cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)\n\n";

        MatrizBase<float>* matrizC = matrizA->sumar(*matrizB);

        std::cout << "Matriz Resultado C (3x2, Tipo FLOAT):\n";
        matrizC->imprimir();
        std::cout << "\n";

        std::cout << "Demostracion de Limpieza de Memoria\n";
        std::cout << "Llamando al destructor de C\n";
        delete matrizC;
        std::cout << "Liberando memoria de C (Matriz Dinamica)\n";

        std::cout << "Llamando al destructor de A\n";
        delete matrizA;
        std::cout << "Liberando memoria de A (Matriz Dinamica)\n";

        std::cout << "Llamando al destructor de B\n";
        delete matrizB;
        std::cout << "Liberando memoria de B (Matriz Estatica)\n";

        std::cout << "Sistema cerrado.\n\n";

        std::cout << "Demostracion con Operador + Sobrecargado\n\n";

        MatrizBase<int>* matrizX = new MatrizDinamica<int>(2, 2);
        matrizX->establecerElemento(0, 0, 1); matrizX->establecerElemento(0, 1, 2);
        matrizX->establecerElemento(1, 0, 3); matrizX->establecerElemento(1, 1, 4);

        MatrizBase<int>* matrizY = new MatrizDinamica<int>(2, 2);
        matrizY->establecerElemento(0, 0, 5); matrizY->establecerElemento(0, 1, 6);
        matrizY->establecerElemento(1, 0, 7); matrizY->establecerElemento(1, 1, 8);

        std::cout << "Matriz X:\n";
        matrizX->imprimir();
        std::cout << "\nMatriz Y:\n";
        matrizY->imprimir();

        std::cout << "\nUsando operador +: Z = X + Y\n";
        MatrizBase<int>* matrizZ = (*matrizX) + (*matrizY);

        std::cout << "\nMatriz Z (resultado):\n";
        matrizZ->imprimir();

        delete matrizX;
        delete matrizY;
        delete matrizZ;

    } catch(const std::exception& excepcion) {
        std::cerr << "Error: " << excepcion.what() << std::endl;
        return 1;
    }

    return 0;
}
