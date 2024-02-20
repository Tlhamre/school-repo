#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORDS 700
#define MAX_WORD_LENGTH 50
#define MAX_SENTENCE 500
#define EPOCH 1000

void gradientDescent(int oneHot[MAX_WORDS][MAX_WORDS], double w[MAX_WORDS], int *labels, int sentenceCount, int wordCount);
void readSentencesAndDictionary(const char *filename, char dictionary[MAX_WORDS][MAX_WORD_LENGTH], int *wordCount, int oneHot[MAX_WORDS][MAX_WORDS], int *sentenceCount);
void processSentence(char dictionary[MAX_WORDS][MAX_WORD_LENGTH], int *wordCount, const char *sentence);
void addToDictionary(char dictionary[MAX_WORDS][MAX_WORD_LENGTH], int *wordCount, const char *word);
void oneHotVector(char dictionary[MAX_WORDS][MAX_WORD_LENGTH], int oneHot[MAX_WORDS][MAX_WORDS], int *wordCount, const char *sentence, int *oi);
void stochasticGradientDescent(int oneHot[MAX_WORDS][MAX_WORDS], double w[MAX_WORDS], int *labels, int sentenceCount, int wordCount);
void adamFunc(int oneHot[MAX_WORDS][MAX_WORDS], double w[MAX_WORDS], int *labels, int sentenceCount, int wordCount);
void testFunc(int oneHot[MAX_WORDS][MAX_WORDS], double w[MAX_WORDS], int sentenceCount, int wordCount);

int main()
{
    int wordCount = 0, sentenceCount, i, j, len, oi = 0, label[MAX_SENTENCE];
    double w[MAX_WORDS];
    char dictionary[MAX_WORDS][MAX_WORD_LENGTH], sentence[MAX_SENTENCE];
    int oneHot[MAX_WORDS][MAX_WORDS] = {0};
    double randomValue;
    double result;
    srand(time(NULL));
    for (i = 0; i < MAX_WORDS; ++i)
    {
        randomValue = (double)rand() / RAND_MAX;
        result = 2 * randomValue - 1;
        w[i] = result;
    }
    readSentencesAndDictionary("text1.txt", dictionary, &wordCount, oneHot, &sentenceCount);

    for (i = 0; i < 90; i++)
    {
        label[i] = 1;
    }
    for (i = 90; i < 180; i++)
    {
        label[i] = -1;
    }

    FILE *file = fopen("text1.txt", "r");
    if (file == NULL)
    {
        printf("Dosya açma hatasi");
        return -1;
    }

    // Dosyadan cümleleri oku
    while (fgets(sentence, sizeof(sentence), file) != NULL)
    {
        // Cümlenin sonundaki newline karakterini kaldır
        len = strlen(sentence);
        if (len > 0 && sentence[len - 1] == '\n')
        {
            sentence[len - 1] = '\0';
        }

        oneHotVector(dictionary, oneHot, &wordCount, sentence, &oi);
    }

    //take out only one function out of comment line to use
    // gradientDescent(oneHot, w, label, sentenceCount, wordCount);
    stochasticGradientDescent(oneHot, w, label, sentenceCount, wordCount);
    // adamFunc(oneHot, w, label, sentenceCount, wordCount);
    testFunc(oneHot, w, sentenceCount, wordCount);
   
   
    fclose(file);
    return 0;
}

// Gradient descent fonksiyonu.
void gradientDescent(int oneHot[MAX_WORDS][MAX_WORDS], double w[MAX_WORDS], int *labels, int sentenceCount, int wordCount)
{
    int i = 0, j, k, l;
    double loss;
    double wx;
    FILE *dosya = fopen("GD_output.txt", "w");

    if (dosya == NULL)
    {
        printf("Dosya açma hatasi!");
        exit(1); // Hata durumunda programı sonlandır
    }

    FILE *dosya2 = fopen("w2.txt","w");
    if (dosya2 == NULL)
    {
        printf("Dosya açma hatasi!");
        exit(1); // Hata durumunda programı sonlandır
    }
    while (i < EPOCH)
    {
        loss = 0;
        // Son 20 cümle test cümlesi olduğu için senteceCountun 20 eksiği kadar döndürür
        for (j = 0; j < sentenceCount - 20; j++)
        {
            wx = 0;
            // tanhta kullanmak için wxi hesaplar
            for (k = 0; k < wordCount; k++)
            {
                wx = wx + (oneHot[j][k] * w[k]);
            }
            

            loss += (double)((labels[j] - tanh(wx)) * (labels[j] - tanh(wx)));

            // w'yü hesaplar
            for (k = 0; k < wordCount; k++)
            {
                w[k] += (double)(0.05 * oneHot[j][k] * (labels[j] - tanh(wx)) * (1.0 / (pow(cosh(wx), 2))));
                //  for ( i = 0; i < wordCount; i++)
                //  {
                //     fprintf(dosya2, "%lf ",w[i]);
                //  }
                // fprintf(dosya2, "\n");
                
                
            }
        }

        fprintf(dosya, "%d\t%lf\n", i+1, loss / sentenceCount);

      
        i++;
    }
    fclose(dosya);
    fclose(dosya2);
}
// Dosyadan cümleleri okuyup kelimeleri sözlüğe ekler
void readSentencesAndDictionary(const char *filename, char dictionary[MAX_WORDS][MAX_WORD_LENGTH], int *wordCount, int oneHot[MAX_WORDS][MAX_WORDS], int *sentenceCount)
{
    int i;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Dosya açma hatasi");
        exit(1);
    }

    char sentence[MAX_SENTENCE];
    int len;

    // Dosyadan cümleleri oku
    while (fgets(sentence, sizeof(sentence), file) != NULL)
    {
        // Cümlenin sonundaki newline karakterini kaldır
        len = strlen(sentence);
        if (len > 0 && sentence[len - 1] == '\n')
        {
            sentence[len - 1] = '\0';
        }

        // Cümleyi işle ve kelimeleri sözlüğe ekle
        processSentence(dictionary, wordCount, sentence);
        (*sentenceCount)++;
    }

    fclose(file);
}

// Verilen cümleyi işleyip kelimeleri sözlüğe ekler
void processSentence(char dictionary[MAX_WORDS][MAX_WORD_LENGTH], int *wordCount, const char *sentence)
{
    char *token, *saveptr;
    int i;

    // Cümleyi kopyala
    char tempSentence[MAX_SENTENCE];
    strcpy(tempSentence, sentence);

    // Cümleyi parçala ve kelimeleri sözlüğe ekle
    token = strtok_r(tempSentence, " ,.?!;:", &saveptr);
    while (token != NULL)
    {
        // Küçük harfe çevir
        for (i = 0; token[i]; ++i)
        {
            token[i] = tolower(token[i]);
        }

        addToDictionary(dictionary, wordCount, token);
        token = strtok_r(NULL, " ,.?!;:", &saveptr);
    }
}

// Verilen kelimeyi sözlüğe ekler
void addToDictionary(char dictionary[MAX_WORDS][MAX_WORD_LENGTH], int *wordCount, const char *word)
{
    int i, j;

    // Kelime zaten sözlükte mi kontrol et
    for (i = 0; i < *wordCount; ++i)
    {
        if (strcmp(dictionary[i], word) == 0)
        {
            return; // Kelime zaten sözlükte var
        }
    }

    // Kelime sözlükte yok, ekleyebiliriz
    strcpy(dictionary[*wordCount], word);
    (*wordCount)++;
}

// one-hot vectötleri oluşturur.
void oneHotVector(char dictionary[MAX_WORDS][MAX_WORD_LENGTH], int oneHot[MAX_WORDS][MAX_WORDS], int *wordCount, const char *sentence, int *oi)
{
    char tempSentence[MAX_SENTENCE], *token, *saveptr;
    int i;
    strcpy(tempSentence, sentence);

    // bi cümledeki her kelimeyi sözlükle karşılaştırır ona göre 1 koyar
    token = strtok_r(tempSentence, " ,.?!;:", &saveptr);
    while (token != NULL)
    {

        for (i = 0; i < *wordCount; i++)
        {
            if (strcasecmp(token, dictionary[i]) == 0)
            {
                if (*oi < MAX_WORDS && i < MAX_WORDS)
                {
                    oneHot[*oi][i] = 1;
                }
            }
        }
        token = strtok_r(NULL, " ,.?!;:", &saveptr);
    }
    (*oi)++;
}



void stochasticGradientDescent(int oneHot[MAX_WORDS][MAX_WORDS], double w[MAX_WORDS], int *labels, int sentenceCount, int wordCount)
{
    int i, j, k;
    double learningRate = 0.01;
    double wx, loss=0;
    int randomIndex;

    FILE *dosya = fopen("SGD_output.txt", "w");

    if (dosya == NULL)
    {
        printf("Dosya açma hatasi!");
        exit(1); // Hata durumunda programı sonlandır
    }

    for (i = 0; i < EPOCH; ++i)
    
    {
        loss = 0;
        for (j = 0; j < sentenceCount - 20; ++j)
        {
            // Rastgele bir örnek seçimi
            randomIndex = rand() % (sentenceCount - 20);

            wx = 0;

            for (k = 0; k < wordCount; ++k)
            {
                wx += oneHot[randomIndex][k] * w[k];
            }

            loss += (labels[randomIndex] - tanh(wx)) * (labels[randomIndex] - tanh(wx));

            for (k = 0; k < wordCount; ++k)
            {
                w[k] += learningRate * oneHot[randomIndex][k] * (labels[randomIndex] - tanh(wx)) * (1.0 / (pow(cosh(wx), 2)));
            }
        }
        // fprintf(dosya, "%d\t%lf\n", i+1,loss / sentenceCount);

     
    }
    fclose(dosya);
}

void adamFunc(int oneHot[MAX_WORDS][MAX_WORDS], double w[MAX_WORDS], int *labels, int sentenceCount, int wordCount)
{
    double learningRate = 0.001;
    double beta1 = 0.2;
    double beta2 = 0.367;
    double epsilon = 1e-8;
    int i;
    double m[MAX_WORDS] = {0}; // Birinci moment vektörü
    double v[MAX_WORDS] = {0}; // İkinci moment vektörü
    double t = 1;              // Güncelleme sayısı

    double wx, loss = 1, gradient;
    double pred = 0.00001;

    FILE *dosya = fopen("ADAM_output.txt", "w");

    if (dosya == NULL)
    {
        printf("Dosya açma hatasi!");
        exit(1); // Hata durumunda programı sonlandır
    }

    for (int epoch = 0; epoch < EPOCH ; ++epoch)
    {
        for (int i = 0; i < sentenceCount - 20; ++i)
        {
            wx = 0;

            for (int j = 0; j < wordCount; ++j)
            {
                wx += oneHot[i][j] * w[j];
            }

            loss = (double)(labels[i] - tanh(wx)) * (labels[i] - tanh(wx));

            // Gradient hesapla
            for (int j = 0; j < wordCount; ++j)
            {
                gradient = oneHot[i][j] * (labels[i] - tanh(wx)) * (1.0 / (pow(cosh(wx), 2)));

                // Moment vektörlerini güncelle
                m[j] = beta1 * m[j] + (1 - beta1) * gradient;
                v[j] = beta2 * v[j] + (1 - beta2) * (gradient * gradient);

                // Bias düzeltmelerini hesapla
                double m_hat = m[j] / (1 - pow(beta1, t));
                double v_hat = v[j] / (1 - pow(beta2, t));

                // Ağırlığı güncelle
                w[j] += (double)(learningRate * m_hat) / (sqrt(v_hat) + epsilon);
            }

            // Güncelleme sayısını arttır
            ++t;
        }

       
    }
}

//test kümesindeki örneklerin değerlerini döndüren fonksiyon
void testFunc(int oneHot[MAX_WORDS][MAX_WORDS], double w[MAX_WORDS], int sentenceCount, int wordCount){
    int i, j, k=1;
    double wx;
    //test kümesindeki örneklerin wx'lerini hesaplayan ve tanh'larını alıp yazdıran döngü
    for ( i = 180; i < sentenceCount; i++)
    {
        wx=0;
        for ( j = 0; j < wordCount; j++)
        {
            wx += oneHot[i][j] * w[j];
        }
        printf("Ornek %d cikti: %lf\n ", k, tanh(wx));
        k++;
    }
    

}