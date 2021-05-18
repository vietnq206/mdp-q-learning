#include <iostream>
#include <vector>
#include <set>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <string>


using namespace std;

struct Location{
    int X;
    int Y;
    float U;
    char D ='^';
    Location(){};
    Location(int x, int y){ X = x; Y = y;};
    Location(int x, int y, float u){ X = x; Y = y; U = u;};
    
    Location(int x, int y, float u, char d){ X = x; Y = y; U = u; D = d;};

};


float checkConverge(vector<Location> U1, vector<Location> U2)
{
    int length = U1.size()-2;
    float converge = 0;
    //cout<<converge<<endl;
    for ( int i =0;i<length;i++){
         converge += abs(pow(U1[i].U,2) - pow(U2[i].U,2));

    }

    return converge;
}
Location checkNormalState(Location s, char d, vector<Location> state)
{
    int xtmp,ytmp;
    switch (d)
    {
    case '^' :
        {
            xtmp =0;
            ytmp =1;
            break;
        }
    case 'v' :
        {
            xtmp =0;
            ytmp =-1;
            break;
        }
    case '<' :
        {
            xtmp =-1;
            ytmp =0;
            break;
        }
    case '>' :
        {
            xtmp =1;
            ytmp =0;
            break;
        }
    default:
        {
            cout<<"ERROR!!"<<endl;
        }
    }


    for (int i=0; i< state.size();i++)
    {
        if(state[i].X == s.X + xtmp && state[i].Y == s.Y + ytmp  )
        {
             return state[i];
        }
    }
    return s;

}

bool notInSet(int x, int y, vector<Location> T)
{
    for ( int i=0;i<T.size();i++)
    {
        if(T[i].X==x && T[i].Y==y) return false;
    }

    return true;
}

Location returnElm(int x, int y, vector<Location> T)
{
  for(int i=0; i<T.size();i++)
    {
      if(T[i].X == x&& T[i].Y == y)
        return T[i];
    }
    
}


int main(int argc, char **data)
{
    vector<Location> state, world, stateTmp;
    vector<float>  P = {0,0,0,0};
    float R,G;
    Location W,S;
    vector<Location> T,F;
    cout<<data[1]<<endl;
    ifstream myfile;
    myfile.open(data[1],ios::in);
     char Parametr;
     int x,y;
     float u;
   while(1)
   {
         if(myfile.eof())
       {
           break;
       }
       myfile>>Parametr;
        //cout<<Parametr<<endl;


        switch(Parametr)
        {
        case 'W':
            {

                myfile>>x>>y;

                W.X =x;
                W.Y =y;
               // W1(x,y);
                break;
            }
        case 'S':
            {

                myfile>>x>>y;
                S.X =x;
                S.Y =y;
                break;
            }
        case 'P':
            {
                float P1,P2,P3;
                myfile>>P[0]>>P[1]>>P[2];
                break;
            }
        case 'R':
            {
                myfile>>R;
                break;
            }
        case 'G':
            {
                myfile>>G;
                break;
            }
        case 'T':
            {
                myfile>>x>>y>>u;
                T.push_back({x,y,u,'T'});
                break;
            }
        case 'F':
            {
                myfile>>x>>y;
                F.push_back({x,y,0,'F'});
                break;
            }
        default:
            {
                break;
            }
        }



   }
   myfile.close();

    P[3] = 1 - P[1] - P[0] - P[2];

  cout<<W.X<<W.Y<<endl;

    for ( int j=1;j<W.Y+1;j++)
        for( int i=1;i<W.X+1;i++)
    {
        if(notInSet(i,j,F)&&notInSet(i,j,T))
        {
            state.push_back({i,j,0});
        }

    }
     for( int i=0;i<T.size();i++)
    {
        state.push_back(T[i]);
    }


    for(int i=0;i<state.size();i++)
    {
        cout<<state[i].X<< " "<<state[i].Y<< " "<<state[i].U<< " "<<state[i].D<<endl;
    }

    cout<<P[0]<<P[1]<<P[2]<<endl;



    int dem =0;
    float Umax,Uup,Uleft,Udown,Uright;

    do
    {

       stateTmp = state;
        for ( int i =0; i < state.size()-T.size();i++)
        {
            Location Sup = checkNormalState(state[i],'^',state);
            Location Sdown = checkNormalState(state[i],'v',state);
            Location Sleft = checkNormalState(state[i],'<',state);
            Location Sright = checkNormalState(state[i],'>',state);


             Uup = P[0] * Sup.U + P[1] * Sleft.U + P[2] * Sright.U;
             Uleft = P[0] * Sleft.U + P[1] * Sdown.U + P[2] * Sup.U;
             Uright = P[0] * Sright.U + P[1] * Sup.U + P[2] * Sdown.U;
             Udown = P[0] * Sdown.U + P[1] * Sleft.U + P[2] * Sright.U;

             Umax = -1;

            if ( Uup > Umax) {Umax = Uup; state[i].D = '^';}
            if ( Udown > Umax) {Umax = Udown;state[i].D = 'v';}
            if ( Uleft > Umax) {Umax = Uleft;state[i].D = '<';}
            if ( Uright > Umax) {Umax = Uright;state[i].D = '>';}

            state[i].U = R + Umax;

        }

        //cout<<checkConverge(state,stateTmp)<<endl;
        dem++;
    }while(checkConverge(state,stateTmp) > 0.0001);

    world.insert(world.end(),state.begin(),state.end());
    world.insert(world.end(),F.begin(),F.end());
    
    
    for ( int i =0; i < world.size();i++)
        cout<<"("<<world[i].X<<","<<world[i].Y<<") - U ="<<setprecision(3)<<world[i].U<<" and policy : "<<world[i].D<<endl;


        cout<<F[0].D<<endl;
        int M=5,N=6;

        for ( int i=0;i<3*N+1;i++)

        {
            if(i%3 ==0)
            for ( int j=0;j<M;j++)
                {
            cout<<"----------";

                }
            else
              for ( int j=0;j<M;j++)
                {

            cout<<"         |";

                }

        cout<<endl;

        }






    return 0;

}

