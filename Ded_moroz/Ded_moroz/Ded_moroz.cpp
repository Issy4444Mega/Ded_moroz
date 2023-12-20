// Ded_moroz.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int calculate_Children_Happiness(int columnNumber);
int calculate_Dedmoroz_Happiness(int columnNumber);

struct Gift {
    int giftID;
    std::vector<int> childrenIDs;
};

struct ChildWishes {
    int childID;
    std::vector<int> desiredGifts;
};

struct rezult {
    int childID_rez;
    int giftID_rez;
};

double UNIS_all_happiness = 0;
const int numChildren = 184000; // Количество детей
const int numGifts = 1000; // Общее количество различных подарков
const int maxGiftsOfType = 185; // Максимальное количество каждого вида подарков
// Функция для чтения данных из файла и сохранения в динамический массив

Gift** readGiftData(const std::string& file_Ded_Moroz) {
    // Создаем двумерный динамический массив для хранения подарков
    Gift** gifts = new Gift * [numGifts];
    for (int i = 0; i < numGifts; ++i) {
        gifts[i] = new Gift[maxGiftsOfType];
    }

    // Чтение данных из файла
    std::ifstream file(file_Ded_Moroz);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл " << file_Ded_Moroz << std::endl;
        return nullptr;
    }

    std::string line;
    int giftCount[numGifts] = { 0 }; // Счетчик подарков каждого вида

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        int giftID = 0;
        if (std::getline(iss, token, ',')) {
            giftID = std::stoi(token);
        }

        if (giftCount[giftID] < maxGiftsOfType) {
            gifts[giftID][giftCount[giftID]].giftID = giftID;

            while (std::getline(iss, token, ',')) {
                gifts[giftID][giftCount[giftID]].childrenIDs.push_back(std::stoi(token));
            }

            giftCount[giftID]++;
        }
    }

    file.close();
    return gifts;
}


ChildWishes** readWishData(const std::string& file_Kids_wish) {
    // Создаем двумерный динамический массив для хранения желаний детей
    ChildWishes** childrenWishes = new ChildWishes * [numChildren];
    for (int i = 0; i < numChildren; ++i) {
        childrenWishes[i] = new ChildWishes;
    }

    // Чтение данных из файла
    std::ifstream file(file_Kids_wish);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл " << file_Kids_wish << std::endl;
        return nullptr;
    }

    std::string line;
    int currentChild = 0;

    while (std::getline(file, line) && currentChild < numChildren) {
        std::istringstream iss(line);
        std::string token;

        if (std::getline(iss, token, ',')) {
            childrenWishes[currentChild]->childID = std::stoi(token);
        }

        while (std::getline(iss, token, ',')) {
            childrenWishes[currentChild]->desiredGifts.push_back(std::stoi(token));
        }

        ++currentChild;
    }

    file.close();
    return childrenWishes;
}


void writeWishData(const std::string& filename, rezult** childrenWishes, int numRecords) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << std::endl;
        return;
    }

    file << "ChildId,GiftId\n"; 

    for (int i = 0; i < numRecords; ++i) {
        file << childrenWishes[i]->giftID_rez << "," << childrenWishes[i]->childID_rez << "\n"; // Записываем данные каждого ребенка и подарка
    }

    file.close();
}



//функция проверяет остались ли подарки данного типа
bool check_gift(int gift)
{
    if (gift < 1) 
    {
        return false;
    }
    else 
    {
        return true;
    }
}

bool check_gift_bliz(int gift)
{
    if (gift < 2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//функция дарения
int give_gift(rezult** itog_rezult, int gift_id, int kid_id)
{
    itog_rezult[kid_id]->childID_rez = gift_id;
    itog_rezult[kid_id]->giftID_rez = kid_id;
    return 1;

}

//функция проверяет есть ли подарок который хочет ребенок у деда мороза
int check_dedmoroz_bliznech(Gift** gifts, int  gift_odinak_bliznech[][3], int k_bl , int id_1, int id_2 , int  gift_kolvo[])
{
    int max_happy_id;// 1-й id подарка 2-й счастье
    int max_happy[1][5] = { -1,-5,-5,-5,-5}; //1-й id подарка  2-й место 1-го в списке деда мороза 3-й место 2-го в списке деда мороза
    double unis = 0;
 
    for (int i = 0; i < k_bl; i++)
    {
        int save_happy[1][3] = { -1,-1,-1 }; //1-й id подарка  2-й место 1-го в списке деда мороза 3-й место 2-го в списке деда мороза
        bool first = false;
        bool second = false;
        save_happy[0][0] = gift_odinak_bliznech[i][0];
        for (int j = 0; j < 184; j++)
        {
            if (gifts[gift_odinak_bliznech[i][0]]->childrenIDs[j] == id_1 && first == false)
            {
                save_happy[0][1] = calculate_Dedmoroz_Happiness(j + 1);
                first = true;
            }
            if (gifts[gift_odinak_bliznech[i][0]]->childrenIDs[j] == id_2 && second == false)
            {
                save_happy[0][2] = calculate_Dedmoroz_Happiness(j + 1);
                second = true;
            }
            if (second == true && first == true)
            {
                break;
            }
        }
        double first_un = ((double)save_happy[0][1] + (double)save_happy[0][2])  + ((double)calculate_Children_Happiness(gift_odinak_bliznech[i][1]) + (double)calculate_Children_Happiness(gift_odinak_bliznech[i][2]));
        double second_un = ((double)max_happy[0][1]  + (double)max_happy[0][2]) + ((double)calculate_Children_Happiness(max_happy[0][3])  + (double)calculate_Children_Happiness(max_happy[0][4]));
        if ((first_un > second_un) && check_gift_bliz(gift_kolvo[gift_odinak_bliznech[i][0]]) == true)
        {
            max_happy_id = save_happy[0][0];
            unis = (double)save_happy[0][1] / 1000 + (double)save_happy[0][2] / 1000 + (double)calculate_Children_Happiness(gift_odinak_bliznech[i][1]) / 184000 + (double)calculate_Children_Happiness(gift_odinak_bliznech[i][2]) / 184000;
            max_happy[0][1] = save_happy[0][1];
            max_happy[0][2] = save_happy[0][2];
        }
    }
    UNIS_all_happiness += unis;
    return max_happy_id;
}

int calculate_Children_Happiness(int columnNumber) //bool это проверкаа из check
{
    if (columnNumber <= -1) {
        return -1; 
    }
    return 2 * (100 - columnNumber);
}

int calculate_Dedmoroz_Happiness(int columnNumber) //bool это проверкаа из check
{
    if (columnNumber == -1) {
        return -1; 
    }

    return 2 * (184 - columnNumber);
}

int check_dedmoroz(Gift** gifts, int id_gift, int id_kids, int place_gift) // получаем индекс если подарок находится в списке подарка
{
    for (int i = 0; i < 184; i++) 
    {
        if (gifts[id_gift]->childrenIDs[i] == id_kids)
        {
            return i + 1;
        }
    }
    return -1;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    const int numRecords = 184000; //количества записей

    rezult** itog_rezult = new rezult * [numRecords];// итоговый массив
    for (int i = 0; i < numRecords; ++i) {
        itog_rezult[i] = new rezult; // Выделение памяти для каждой структуры в массиве
    }
    const std::string file_Ded_Moroz = "ded_moroz_wish.csv";
    const std::string file_Kids_wish = "kids_wish.csv";
    const std::string file_rez = "output_kids_wish.csv";
    
    // Вызов функции для чтения данных
    Gift** gifts = readGiftData(file_Ded_Moroz);
    ChildWishes** childrenWishes = readWishData(file_Kids_wish);


    int gift_kolvo[1000]; // Масив для кол-во оставшихся подарков
    for (int i = 0; i < 1000; i++)
    {
        gift_kolvo[i] = 250;
    }

    
    cout << "Файлы успешно открыт"<<endl;
    for (int i = 0; i < 5520; i+=2) //Близннецы
    {
        int gift_odinak_bliznech[100][3];// 1-id подарка 2-место подарка в у 1-го близнеца 3-место подарка в у 2-го близнеца
        for (int m1 = 0; m1 < 100; m1++) // обнуление
        {
            for (int m2 = 0; m2 < 3; m2++)
            {
                gift_odinak_bliznech[m1][m2] = -1;
            }
        }
        int first_bliznech[100][2];// вторая скобка отвечает за место в списке
        int second_bliznech[100][2];


        for (int k = 0; k < 100; k++)
        {
            first_bliznech[k][0] = childrenWishes[i]->desiredGifts[k];
            second_bliznech[k][0] = childrenWishes[i+1]->desiredGifts[k];
            first_bliznech[k][1] = k + 1;
            second_bliznech[k][1] = k + 1;
        }
        int k_bl = 0;
        for (int k = 0; k < 100; k++)//находит одинаковые и сохраняет их столбики далее проверяем со спиком дед мороза и ищем наиболле подходящие
        {
            for (int  j = 0;  j < 100; j++)
            {
                if (first_bliznech[k][0] == second_bliznech[j][0])
                {
                    gift_odinak_bliznech[k_bl][0] = first_bliznech[k][0];
                    gift_odinak_bliznech[k_bl][1] = first_bliznech[k][1];
                    gift_odinak_bliznech[k_bl][2] = second_bliznech[j][1];
                    k_bl++;
                }
            }
        }
        
        int id_gift = check_dedmoroz_bliznech(gifts, gift_odinak_bliznech, k_bl, i, i + 1, gift_kolvo);
        gift_kolvo[id_gift] = gift_kolvo[id_gift] - give_gift(itog_rezult, id_gift, i);
        gift_kolvo[id_gift] = gift_kolvo[id_gift] - give_gift(itog_rezult, id_gift, i + 1);
        k_bl = 0;
    }
    cout << "Близннецы просчитаны";

    for (int i = 5520; i < 184000; i++) // после близнецов 
    {
        bool ready = false; // разрешение подарить подарок
        int save_info[1][4] = { -1,-1,-1,-1 }; //1-й подарок id 2-й место в списке деда мороза 3-й счастье деда мороза 4-й  счастье ребенка
        int max_happy[1][4] = { -1,-1,-1,-1 }; // максимальное счастье 1-й подарок id 2-й место в списке деда мороза 3-й счастье деда мороза 4-й  счастье ребенка
        for (int k = 0; k < 100; k++) 
        {
            save_info[0][0] = childrenWishes[i]->desiredGifts[k];
            save_info[0][1] = check_dedmoroz(gifts, childrenWishes[i]->desiredGifts[k], i, k + 1);
            save_info[0][2] = calculate_Dedmoroz_Happiness(save_info[0][1]);
            save_info[0][3] = calculate_Children_Happiness(k + 1);
            if (((((double)save_info[0][2] / 1000) + ((double)save_info[0][3]/184000)) > (((double)max_happy[0][2] / 1000) + ((double)max_happy[0][3] / 184000))) && check_gift(gift_kolvo[save_info[0][0]]) == true)
            {
                max_happy[0][0] = save_info[0][0];
                max_happy[0][1] = save_info[0][1];
                max_happy[0][2] = save_info[0][2];
                max_happy[0][3] = save_info[0][3];
                ready = true;
            }
        }

        if (ready == true) 
        {
            gift_kolvo[max_happy[0][0]] = gift_kolvo[max_happy[0][0]] - give_gift(itog_rezult, max_happy[0][0], i);
            UNIS_all_happiness += ((double)max_happy[0][2] / 1000) + ((double)max_happy[0][3] / 184000);
        }
        
    } 
    cout << endl;
    cout << UNIS_all_happiness << endl;
    


    writeWishData(file_rez, itog_rezult, numRecords);
    cout << "В файл записаны данные" << endl;
    for (int i = 0; i < numGifts; ++i) {
        delete[] gifts[i];
    }
    delete[] gifts;
    for (int i = 0; i < numChildren; ++i) {
        delete childrenWishes[i];
    }
    delete[] childrenWishes;
    for (int i = 0; i < numRecords; ++i) {
        delete itog_rezult[i];
    }
    delete[] itog_rezult;
    
   
        
   system("pause");
}


