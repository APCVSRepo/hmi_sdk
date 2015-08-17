#include "include/threaddlg.h"
#include <QApplication>
#include "include/json/json.h"
#include <iostream>
#include <string>

#include <map>

#include <QDebug>

#include <fstream>
#include <cassert>

int main(int argc, char *argv[])
{

    //    std::ofstream out("/home/jack/log/json.txt",std::ios::app);

    //    out<< "send:" <<data.toStyledString()<<std::endl;
    //    out.close();

    //    std::ifstream ifs;
    //    ifs.open("config/VehicleInfo.json");
    //    assert(ifs.is_open());

    //    Json::Reader reader;
    //    Json::Value VehicleInfoJsonObj;
    //    Json::Value data;
    //    Json::Value params;
    //    std::string errortext = "Params are not avaliable";
    //    int id;

    //    Json::Value vehicle;

    //  params = message["params"];
    //  id = message["id"].asInt();

    //    if (!reader.parse(ifs, VehicleInfoJsonObj, false))
    //    {
    //        std::cout << "staticConfigJson error.\n";
    //    }
    //    else
    //    {
    //        vehicle = VehicleInfoJsonObj["vehicle"];
    //    }
    //    ifs.close();

    //    Json::Value::Members mem = vehicle.getMemberNames();
    //    for (Json::Value::Members::iterator iter = mem.begin(); iter != mem.end(); iter++)
    //    {
    //        std::cout<<*iter<<":\t";
    //        std::cout<<vehicle[std::string(*iter)].toStyledString() << "\n";
    //    }
    //  return 0;



    QApplication a(argc, argv);
    ThreadDlg w;
    w.show();
    return a.exec();
}


//{
//   "id" : 64,
//   "jsonrpc" : "2.0",
//   "method" : "Navigation.StartStream",
//   "params" : {
//      "appID" : 846930886,
//      "url" : "/tmp/video_stream_pipe"
//   }
//}


//---------
//+++++recvFromServer+++++
//{
//   "id" : 65,
//   "jsonrpc" : "2.0",
//   "method" : "Navigation.StopStream",
//   "params" : {
//      "appID" : 846930886
//   }
//}

//+++++recvFromServer+++++
//{
//   "id" : 44,
//   "jsonrpc" : "2.0",
//   "method" : "Navigation.StartStream",
//   "params" : {
//      "appID" : 846930886,
//      "url" : "http://127.0.0.1:5050"
//   }
//}
