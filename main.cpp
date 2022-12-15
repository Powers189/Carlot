#include <iostream>
#include <cmath>
#include "Event.h"
#include <vector>
#include <queue>
#include <random>
#include "Random.h"
#include <fstream>
#include "ArrivalObject.h"
using namespace std;

bool Is_empty(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

double Uniform(double a, double b, double u)
{
    return (a + (b - a) * u);
}
int Equalikely(int a, int b, double u)
{
    return (a + floor((b - a + 1) * u));
}

double Exp_Trunc(double mu, double lower_limit, double upper_limit, double rand)
{
    double u = Uniform(lower_limit, upper_limit, rand);
    return -mu * log(1 - u);
}

double Triangular(double a, double b, double c, double u)
{
    if (u < ((c - a) / (b - a)))
    {
        return a + sqrt((b - a) * (c - a) * u);
    }
    else
    {
        return b - sqrt((b - a) * (b - c) * (1 - u));
    }
}

int Bernoulli(double p)
{
    static std::default_random_engine generator(6);
    std::bernoulli_distribution distribution(p);
    return distribution(generator);
}

double DeliveryLag(double previous_order_time, double q, double a, double b, double c, double u, double t)
{
    double max;
    max = previous_order_time;
    if (t > previous_order_time)
    {
        max = t;
    }
    // q=num cars
    return max + ((387 + q) / 13) + 23.7 + Triangular(a, b, c, u);
}
double Exponential(double mu, double u)
{
    return -mu * log(1 - u);
}
double nextArrival(double alpha_i, vector<ArrivalObject> I, double u)
{
    double last = I.size() - 1;
    double tk = I[last].tright;
    double cum_tk = I[last].cumright;
    double a_i = alpha_i - (floor(double(alpha_i) / tk) * tk);
    double j_i = 0;
    while (a_i > I[j_i].tright)
    {
        j_i++;
    }
    double cum_a_i = I[j_i].cumleft + (I[j_i].lamda * (a_i - I[j_i].tleft));
    double cum_a_iplus1 = cum_a_i + Exponential(1, u);
    double t_cycle = 0;
    double w;
    //  cout<< "cumAiplus1:"<<cum_a_iplus1<<" cum_ai:"<<cum_a_i<<"  cum_tk:"<<cum_tk<<endl;
    if (cum_a_iplus1 >= cum_tk)
    {
        w = floor((cum_a_iplus1 - cum_a_i) / cum_tk);
        // cout<< "W: "<< w<<endl;
        cum_a_iplus1 = cum_a_iplus1 - (w * cum_tk);
        t_cycle = t_cycle + (w * tk);
        // cout<<"TTT cycle"<<t_cycle<<endl;
        cum_a_iplus1 = cum_a_iplus1 - cum_tk;
        // cout<<"    tk: "<<tk<<"    a_i: "<<a_i<<endl;
        t_cycle = t_cycle + (tk - a_i);
        j_i = 0;
    }
    // cout<<"t cycle"<<t_cycle<<endl;
    double j_i_plus1 = j_i;
    while (cum_a_iplus1 > I[j_i_plus1].cumright)
    {
        j_i_plus1++;
    }
    double a_iplus1 = ((cum_a_iplus1 - I[j_i_plus1].cumleft) / (I[j_i_plus1].lamda)) + I[j_i_plus1].tleft;
    if (t_cycle > 0)
    {
        // cout<<"DId I ever get here.....";
        return alpha_i + t_cycle + a_iplus1;
    }
    else
    {
        // cout<<"Ar time: "<<alpha_i+t_cycle+alpha_i;
        return alpha_i + (a_iplus1 - a_i);
    }
}

int main(int argc, char *argv[])
{
    // TRIANGLE
    string run_mode = argv[1];
    if (run_mode == "TRIANGLE")
    {
        string random_file = argv[2];
        Random RANDY(random_file);
        if (!RANDY.isOpen())
        {
            return -1;
        }
        double rando;
        rando = RANDY.GetRandom();
        if (rando == -1)
        {
            return -1;
        }

        // rand_file.open(random_file);
        double a = stod(argv[4]);
        double b = stod(argv[5]);
        double c = stod(argv[6]);
        double u;
        int count = 0;
        while (rando != -1)
        {
            cout << "OUTPUT " << Triangular(a, b, c, rando) << endl;
            rando = RANDY.GetRandom();
        }
    }

    else if (run_mode == "ARRIVALS")
    {
        string random_file = argv[2];
        Random RANDY(random_file);
        if (!RANDY.isOpen())
        {
            return -1;
        }
        double rando;
        rando = RANDY.GetRandom();

        string lamda_file = argv[3];
        ifstream Lamda_file;
        Lamda_file.open(lamda_file);
        if (Lamda_file.is_open())
        {
            // fprintf(stdout, "opened file\n");
        }
        else
        {
            return -1;
        }
        if (Lamda_file.eof())
        {
            return -1;
        }
        double lamda_start = 0;
        double lamda_end;
        double lamda;
        vector<ArrivalObject> I;
        double L = 0;
        double l = 0;
        while (Lamda_file >> lamda_end >> lamda)
        {
            ArrivalObject arrival_object;
            arrival_object.tleft = lamda_start;
            arrival_object.tright = lamda_end;
            arrival_object.lamda = lamda;
            arrival_object.cumleft = L;
            l = arrival_object.lamda * (arrival_object.tright - arrival_object.tleft);
            L = L + l;
            arrival_object.cumright = L;
            I.push_back(arrival_object);
            // cout<<"start:"<<lamda_start<<" end:"<<lamda_end<<" lamda:"<<lamda<<endl;
            lamda_start = lamda_end;
        }
        double ai = nextArrival(0, I, rando);
        double aiplus1 = 0;

        double count = 0;
        while (rando != -1)
        {
            aiplus1 = nextArrival(ai, I, rando);
            cout << "OUTPUT " << aiplus1 << endl;
            ai = aiplus1;
            rando = RANDY.GetRandom();
        }
    }
    else
    {
        // ofstream outputfile;
        // outputfile.open("penalty_data.csv");

        // for ( int i = 0; i <= 99; i++) {
        // double total_penalties=0;
        // for (int j=0; j<2;j++) {
        string random_file = argv[2];
        Random RANDY(random_file);
        // if (!RANDY.isOpen())
        // {
        //     fprintf(stdout, "random file not open\n");
        //     return -1;
        // }
        string lamda_file = argv[3];
        ifstream Lamda_file;
        Lamda_file.open(lamda_file);
        if (!Lamda_file)
        {
            fprintf(stdout, "bad lamda file\n");
            return -1;
        }
        if (Is_empty(Lamda_file))
        {
            return -1;
        }

        double a = stod(argv[4]);
        double b = stod(argv[5]);
        double c = stod(argv[6]);
        double S = stod(argv[7]);
        double s = stod(argv[8]);
        // double s = i;
        double start_time = stod(argv[9]);
        double end_time = stod(argv[10]);
        double previous_delivery_time = 0;
        double delivery_lag = 0;
        double h = double(2) / (b - a);
        double min_inventory = S;
        int rumorMill_max = 0;
        // Normal uniform-0-1-00.dat lamdas.txt 2 4 5 500 100 0 2000

        double rando;
        rando = RANDY.GetRandom();
        if (rando == -1)
        {
            return -1;
        }

        double lamda_start = 0;
        double lamda_end;
        double lamda;
        vector<ArrivalObject> I;
        double L = 0;
        double l = 0;
        if (Lamda_file.eof())
        {
            return -1;
        }
        while (Lamda_file >> lamda_end >> lamda)
        {
            ArrivalObject arrival_object;
            arrival_object.tleft = lamda_start;
            arrival_object.tright = lamda_end;
            arrival_object.lamda = lamda;
            arrival_object.cumleft = L;
            l = lamda * (arrival_object.tright - arrival_object.tleft);
            L = L + l;
            arrival_object.cumright = L;
            I.push_back(arrival_object);
            // cout<<"start:"<<lamda_start<<" end:"<<lamda_end<<" lamda:"<<lamda<<endl;
            lamda_start = lamda_end;
        }

        priority_queue<Event, vector<Event>, greater<Event>> EventList;
        double simClock = 0.0;
        double inventory = S;
        double onOrder = 0.0;
        double paid_penalties = 0;

        double backorderCount = 0.0; // Number of cars purchased when inventory is <=0
        double orderCount = 0.0;     // Number of orderes placed
        queue<double> backorder_times;

        // First review
        EventList.push(Event(60, "Review Event", 0.0));
        rando = RANDY.GetRandom();
        if (rando == -1)
        {
            return -1;
        }
        EventList.push(Event(nextArrival(0, I, rando), "Demand Event", 0.0));

        // Run until the sim ends
        while (simClock <= end_time && !EventList.empty())
        {
            Event event = EventList.top();
            EventList.pop();
            simClock = event.at;
            // cout << event.type << " " << event.at << " " << inventory << "  on order: " << onOrder << endl;

            if (event.type == "Review Event")
            {

                // schedule next review
                if (simClock + 60 < end_time)
                {
                    EventList.push(Event(simClock + 60, "Review Event", 0));
                }
                if (inventory < min_inventory && simClock >= start_time)
                {
                    min_inventory = inventory;
                }

                double order_amount;
                if (inventory + onOrder <= s)
                {
                    order_amount = S - (inventory + onOrder);
                }
                else
                {
                    order_amount = 0;
                }
                rando = RANDY.GetRandom();
                if (rando == -1)
                {
                    return -1;
                }
                delivery_lag = DeliveryLag(previous_delivery_time, order_amount, a, b, c, rando, simClock);
                Event Delivery = Event(delivery_lag, "Delivery", order_amount);
                if (Delivery.at <= end_time && order_amount != 0)
                {
                    EventList.push(Delivery);
                    orderCount++; // dont need
                    onOrder += order_amount;
                }
            }

            // Subtract the number of delivered cars from the order amount, update inventory
            else if (event.type == "Delivery")
            {
                onOrder -= event.num_cars;
                inventory += event.num_cars;
                double arriving_cars = event.num_cars;
                double penalty = 0;
                while (!backorder_times.empty() && arriving_cars != 0)
                {
                    penalty = floor((simClock - backorder_times.front()) / 10) * 100;
                    // cout<<"timeplaced: "<<backorder_times.front()<< " time arrived: "<<simClock<<endl;
                    backorder_times.pop();
                    arriving_cars--;
                    if (simClock >= start_time)
                    {
                        // cout<<"penalty: "<<penalty<<endl;
                        paid_penalties += penalty;
                        // cout<<"paid penalties"<<paid_penalties<<endl;
                    }
                }
            }
            else if (event.type == "Demand Event")
            {
                if (inventory <= 0)
                {
                    int rumor_count = 0;
                    backorderCount++;
                    backorder_times.push(event.at);
                    inventory--;
                    rando = RANDY.GetRandom();
                    if (rando == -1)
                    {
                        return -1;
                    }
                    double coinflip = Equalikely(0, 1, rando);
                    while (coinflip == 1)
                    {
                        rumor_count++;
                        rando = RANDY.GetRandom();
                        if (rando == -1)
                        {
                            return -1;
                        }
                        EventList.push(Event(simClock + Uniform(2 * h, 9 * h, rando), "Rumor Demand", 1));
                        rando = RANDY.GetRandom();
                        if (rando == -1)
                        {
                            return -1;
                        }
                        coinflip = Equalikely(0, 1, rando);
                    }

                    if (rumor_count > rumorMill_max)
                    {
                        rumorMill_max = rumor_count;
                    }
                }
                else
                {
                    inventory -= 1;
                }

                if (inventory < min_inventory && simClock > start_time)
                {
                    min_inventory = inventory;
                }

                // Generate a new demand event if it wont run over sim end
                rando = RANDY.GetRandom();
                if (rando == -1)
                {
                    return -1;
                }
                Event nextDemand = Event(nextArrival(simClock, I, rando), "Demand Event", 0.0);
                if (nextDemand.at < end_time)
                {
                    EventList.push(nextDemand);
                }
            }
            else if (event.type == "Rumor Demand")
            {
                if (inventory <= 0)
                {
                    backorderCount++;
                    backorder_times.push(event.at);
                    inventory--;

                    if (inventory < min_inventory && simClock >= start_time)
                    {
                        min_inventory = inventory;
                    }
                    double rando;
                    rando = RANDY.GetRandom();
                    if (rando == -1)
                    {
                        return -1;
                    }

                    while (Equalikely(0, 1, rando) == 1)
                    {
                        rando = RANDY.GetRandom();
                        if (rando == -1)
                        {
                            return -1;
                        }
                        h = Uniform(2 * h, 9 * h, rando);
                        EventList.push(Event(simClock + h, "Rumor Demand", 1));
                        rando = RANDY.GetRandom();
                        if (rando == -1)
                        {
                            return -1;
                        }
                    }
                }
            }
        }
        cout << "test";

        cout
            << "OUTPUT MININVENTORY " << min_inventory << endl;
        cout << "OUTPUT PENALTIES " << paid_penalties << endl;
        cout << "OUTPUT MAXRUMORMILL " << rumorMill_max << endl;
        cout << "OUTPUT ORDERS " << orderCount << endl;
        // total_penalties=+paid_penalties;

        // }
        // outputfile<<i<<","<<double(total_penalties)/2<<endl;
        // }
    }

    //    //A uniform-0-1-00.dat lamdas.txt 8 38 12 100 20 2800 5600

    return 0;
}
