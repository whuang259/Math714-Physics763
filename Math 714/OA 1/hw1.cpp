#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

void pascal(int n, bool printParity);
void mat_mul(const double* A, const double* B, double* C, int m, int n, int p);
void conway(int* board, int m, int n);

/** Creates a rectangular matrix where each entry is uniformly
 * distributed between -1 and 1.
 * \param[in] (m,n) the dimensions of the matrix.
 * \return A pointer to the matrix. */
double* gradient_matrix(int m, int n) {
    double *A = new double[m * n];
    for(int i = 0; i < m; i++)
        for(int j = 0 ; j < n; j++)
            A[j + n * i] = j;
    return A;
}

/** Prints the entries of a rectangular matrix. 
 * \param[in] (m,n) the dimensions of the matrix. */
void print_matrix(double *A, int m, int n) {
    for(int i = 0; i < m; i++) {
        printf("%g", A[n * i]);
        for(int j = 1; j < n; j++) printf(" %g", A[n * i + j]);
        putchar('\n');
    }
}

/** Converts an integer array into ASCII art and prints it
 * \param[in] (m,n) the dimensions of the matrix. */
void print_board(int *A, int m, int n) {
    for(int i = 0; i < m; i++) {
        printf("%c", A[n * i] == 1 ? '#' : '.');
        for(int j = 1; j < n; j++) printf(" %c", A[n * i + j] == 1 ? '#' : '.');
        putchar('\n');
    }
}

int main() {
    pascal(7, false);
    putchar('\n');
    pascal(31, true);
    putchar('\n');

    int m = 2, n = 3, p = 4;
    double *A = gradient_matrix(m, n), *B = gradient_matrix(n, p);
    double *C = new double[m * p];
    mat_mul(A, B, C, m, n, p);
    print_matrix(C, m, p);
    putchar('\n');

    m = 80, n = 40;
    int *board = new int[m * n];
    memset(board, 0, m * n * sizeof(int));
    board[3 + n] = 1;
    board[2 + n*2] = 1;
    board[4 + n*2] = 1;
    board[1 + n*3] = 1;
    board[2 + n*3] = 1;
    board[4 + n*3] = 1;
    board[5 + n*3] = 1;
    conway(board, 80, 40);

    return 0;
}

void pascal(int n, bool printParity) {
    int *row = new int[n + 1];
    memset(row, 0, (n + 1) * sizeof(int));
    row[0] = 1;
    for(int i = 0; i <= n; i++) {
        for(int j = i; j >= 0; j--) {
            row[j] += j > 0 ? row[j - 1] : 0;
            
            if(printParity)
                printf("%c ", row[j] % 2 == 0 ? '.' : '#');
            else
                printf("%d ", row[j]);
        }
        putchar('\n');
    }
    delete[] row;
}

void mat_mul(const double* A, const double* B, double* C, int m, int n, int p) {
    for(int i = 0; i < m * p; i++) {
        C[i] = 0;
        for(int j = 0; j < n; j++)
            C[i] += A[(i / p) * m + j] * B[j * p + (i % p)];
    }
}

void conway(int* board, int m, int n) {
    print_board(board, m, n);
    putchar('\n');

    int* newBoard = new int[m * n];
    for(int t = 1; t <= 200; t++) {
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                int N = 0;

                for(int dx = -1; dx <= 1; dx++) {
                    for(int dy = -1; dy <= 1; dy++) {
                        if(dx == 0 && dy == 0)
                            continue;

                        // Add deltas and account for periodic boundaries
                        int row = (i + dx + m) % m;
                        int col = (j + dy + n) % n;
                        N += board[row * n + col];
                    }
                }

                int cur = i * n + j;
                if(board[cur] == 1 && (N == 2 || N == 3))
                    newBoard[cur] = 1;
                else if(board[cur] == 0 && N == 3)
                    newBoard[cur] = 1;
                else
                    newBoard[cur] = 0;
            }
        }

        memcpy(board, newBoard, m * n * sizeof(int));

        if(t == 1 || t == 2 || t == 4 || t == 50 || t == 100 || t == 200) {
            print_board(board, m, n);
            putchar('\n');
        }
    }

    delete[] newBoard;
}