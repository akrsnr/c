// Sayı tahmin
// Aciklama:
// https://raw.githubusercontent.com/akrsnr/pics/master/rifat_colkesen_algoritmalar_ornek.jpg

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// burada controlü 0 yaparak ilk başta üretilen sayının görünmemesini sağlayabilirsiniz.
#define CONTROL  1

// üretilecek sayının kaç basamaklı olduğu
enum { DIGIT_SIZE = 4 };


int randomNumberGenerator(int min, int max);
int checkUniqueness(char const * );
void FourDigitNumber(char * );
int checkExistanceOfDigit(char const * , char c);
void guessIndicator(char const * , char const * , int * , int * );
int isNumber(char const * );


int main() {
    srand(time(NULL));

    char     generatedNumber[DIGIT_SIZE + 1] = { 0 };
    int      right = 0;
    int      wrong = 0;
    char     userInput[DIGIT_SIZE + 1] = { 0 };

    FourDigitNumber(generatedNumber);
    generatedNumber[DIGIT_SIZE] = 0; // null-terminator

#if CONTROL
    puts(generatedNumber);
#endif


    do {
        right = 0;
        wrong = 0;
        printf("Enter a four-digit number: ");
        scanf("%s", userInput);

        if (isNumber(userInput) || checkUniqueness(userInput))
            continue;

        guessIndicator(generatedNumber, userInput, &right, &wrong);
        printf("%.4s  => +%d  -%d\n", userInput, right, wrong);
    } while(strcmp(generatedNumber, userInput));


    return 0;
}

/**
 * Rastgele sayı üretimi
 * Ama biz bunu digit üretimi için kullanıyoruz
 * @param min en küçük sınır(dahil)
 * @param max en büyük sınır(dahil)
 * @return rastgele bir sayı
 */
int randomNumberGenerator(int min, int max) {
    int low = 0;
    int high = 0;

    min < max ?  ( low = min, high = max + 1 )  : ( low = max + 1, high = min );
    return (rand() % (high - low)) + low;
}

/**
 * Girilen numaranın unique olmasını kontrol eder
 * @param arr herhangi bir sayı
 * @return non-unique 1, otherwise 0
 */
int checkUniqueness(char const * arr) {
    for (int i = 0; i < DIGIT_SIZE; ++i) {
        for (int j = i + 1; j < DIGIT_SIZE; ++j) {
            if (arr[i] == arr[j]) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * 4 basamaklı bir *unique* sayı üretilmesini sağlar
 * @param arr 4 haneli karater arrayinde ki sayı
 */
void FourDigitNumber(char * arr) {
    do {
        for (int i = 0; i < DIGIT_SIZE; ++i) {
            arr[i] = (char) ('0' + randomNumberGenerator(0, 9));
        }
    } while (checkUniqueness(arr));
}

/**
 * Verilen rakamın, verilen array de olup olmadığını kontrol eder
 * @param arr verilen sayı
 * @param c verilen rakam
 * @return varsa 1, yoksa 0
 */
int checkExistanceOfDigit(char const * arr, char c) {
    for (int i = 0; i < DIGIT_SIZE; ++i) {
        if(arr[i] == c)
            return 1;
    }
    return 0;
}

/**
 * Girilen sayı ile oluşturulan sayıyı karşılaştırır.
 * Doğru ve yanlış sayısını ayarlar.
 * @param generated oluşturulan sayı
 * @param userInput kullanıcı tarafından girilen sayı
 * @param right eğer sayıda rakamlar doğru yerdeyse her zaman 1 artırılır
 * @param wrong eğer sayıda rakamlar mevcut ama doğru yerde değilse her zaman 1 artırılır
 */
void guessIndicator(char const * generated, char const * userInput, int * right, int * wrong) {
    for (int i = 0; i < DIGIT_SIZE; ++i) {
        generated[i] != userInput[i] ? ( checkExistanceOfDigit(generated, userInput[i]) ? ++*wrong : 0)
                                     : ++*right ;
    }
}

/**
 * Verilenin sayı mı *rakam değil* olup olmadığınu kontrol eder
 * @param arr verilen sayı
 * @return sayı ise 0, otherwise 1
 */
int isNumber(char const * arr) {
    for (int i = 0; i < DIGIT_SIZE; ++i) {
        if(! isdigit(arr[i])) return 1;
    }
    return 0;
}

