#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

CRITICAL_SECTION CriticalSection;

int ilosc_dr = rand ( ) % ( 150 + 1 - 50 ) + 50;//ilosc drozdzy od 50 do 150

struct klient
{
    string name;
    int time;
    int ile_kupil;
    
   // HANDLE thread;
};


klient arr [ 4 ]; ///!!! trzeba ustawiac

void ThreadProc ( klient* arr );
void Guider ( klient* array, int x, HANDLE* ThreadArr );
BOOL done = FALSE;


int main ( int argc , char* argv [ ] )//sklep to program glowny
{

    srand ( time ( 0 ) );
    if (argc < 2)
    {
        printf ( "za malo argumentow" );
        return 1;
    }
   int N = atoi ( argv [ 1 ] );
  // klient* arr = new klient [ N ];
    time_t timeStart = clock ( );
    
    HANDLE* ThreadArr = new HANDLE [ N ];//tablica na watki = klienci
    char tab [ ] = { 'A','B','C','D','E','F','G','H','I','J' };
    InitializeCriticalSection ( &CriticalSection );


    printf ( "Sklep Biedronka: Otwieramy, mamy dzis %d paczek drozdzy. \n" , ilosc_dr );
    
   
        for (int i = 0; i < N; i++)
        {

            arr [ i ].name = tab [ i ];//nadaje imie

            ThreadArr [ i ] = CreateThread ( NULL , 0 , ( LPTHREAD_START_ROUTINE ) ThreadProc , ( LPVOID ) &arr [ i ] , 0 , NULL );
          
            if (ThreadArr [ i ] == NULL)
            {
                ExitProcess ( 3 );
            }
            
           // arr [ i ].thread = ThreadArr [ i ];
            
        }
       /* if (ilosc_dr <= N)
        {
            for (int i = 0; i < N; i++)
                TerminateThread ( ThreadArr [ i ] , 0 );
        }*/
        while (ilosc_dr > 0)
        {
            Sleep ( 300 );
            SetThreadPriority ( Guider , THREAD_PRIORITY_HIGHEST );
            Guider ( arr , N , ThreadArr );
        }
       /* if (ilosc_dr ==N)
        {
            for(int i=0;i<N;i++)
            TerminateThread ( ThreadArr [ i ],0 );
        }*/
    

    time_t wait = 5000 - ((clock()-timeStart)/CLOCKS_PER_SEC);
   Sleep ( wait  );
    WaitForMultipleObjects ( N , ThreadArr , TRUE , INFINITE );

    for (int i = 0; i < N; i++)
    {
        CloseHandle( ThreadArr [ i ]);
    }
    printf ( "Sklep Biedronka: Zamykamy!\n\n\n" );
    printf ( "Statystyki zakupow:\nSklep Biedronka : zostalo %d paczek \n" , ilosc_dr );

    for (int i = 0; i < N; i++)
    {
        if (arr [ i ].ile_kupil == 0)
            cout << "Klient " << arr [ i ].name << ":\t" << "dla mnie zabraklo" << endl;
        else
        cout << "Klient " << arr [ i ].name << ":\t" << arr [ i ].ile_kupil << endl;
        
    }


    delete [ ] ThreadArr;
   // free(arr);
  //  free ( dwThreadIdArray );
    DeleteCriticalSection ( &CriticalSection );
}

void ThreadProc (klient* arr)///watek to klient -> przekazuje w argumencie aktulane liczbe drozdzy
{
   
    //klient* pargs = ( klient* ) pArgs;
  // srand (time(0) );
   // int time_wait = rand ( ) % ( 100 + 1 - 10 ) + 10;//ile czasu zajma klientowi zakupy
    if (ilosc_dr <= 0)
    {
        return;
       // done = TRUE;
    }
   // while (done == FALSE)
    while(ilosc_dr>0)
    {  // Request ownership of the critical section.
        srand ( time ( 0 ) );
        int time_wait = rand ( ) % ( 100 + 1 - 10 ) + 10;//ile czasu zajma klientowi zakupy
        EnterCriticalSection ( &CriticalSection );
        // int ile_kupil = 0;

        Sleep ( time_wait );
        cout << "Klient " << arr->name << ":   kupilem drozdze w " << time_wait << "ms" << endl;
        arr->ile_kupil++;
        //array->time += time_wait;
        ilosc_dr--;
        LeaveCriticalSection ( &CriticalSection );
        if (ilosc_dr <= 0)
        {
            //return;
            return;
        }
        
    }

       // return 1;
}


void Guider ( klient* array , int x , HANDLE* ThreadArr )
{
 
   int  min = array [ 0 ].ile_kupil;//znjduje min ilosc drozdzy u klienta
    
   int  max = array [ 0 ].ile_kupil; //znajduje max ilosc drozdzy u klietna
    for (int i = 1; i < x; i++)
    {
        if (array [ i ].ile_kupil < min)
        {
            min = array [ i ].ile_kupil;
           // ktory_min = i;
        }
        if (array [ i ].ile_kupil > max)
        {
            max = array [ i ].ile_kupil;
            //ktory_max = j;
        }
    }
  


   for (int k = 0; k < x; k++)
   {


       if (max == min)return;
       if (min == array [ k ].ile_kupil)
       {
           SetThreadPriority ( ThreadArr [ k ] , 1 );
           cout << "OCHRONIARZ: Kliencie " << array [ k ].name << " prosze wejsc" << endl;
         //  cout << array[k].ile_kupil << endl;
       }
       if (max == array [ k ].ile_kupil)
       {
           SetThreadPriority ( ThreadArr [ k ] , -2 );
           cout << "OCHRONIARZ: Kliencie " << array [ k ].name << " na koniec" << endl;
           //cout << array[k].ile_kupil << endl;
       }
       else
       {
           SetThreadPriority ( ThreadArr [ k ] , 0 );
       }

   }
}