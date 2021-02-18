
#include "JsonStruct.h"

#include "../json_config.h"
#include <fstream>
#include <iostream>

void printVideoInfo(const VideoInfo& videoInfo)
{
    printf("width: %d\nheight: %d\nframeRate: %d\ncrt: %d\n\n",
        videoInfo.width, videoInfo.height, videoInfo.frameRate, videoInfo.crf);
}

void printTrackInfo(const TrackInfo& trackInfo)
{
    printf("name: %s\nsize: %d\n",
        trackInfo.name.c_str(), trackInfo.size);

    for (int32_t i = 0; i < trackInfo.size; ++i)
    {
        PieceInfo pieceInfo = trackInfo.pieces[i];
        printf("{\n\tfile: %s\n\tstartTime: %d\n\tendTime: %d\n}\n",
            pieceInfo.pathname.c_str(), pieceInfo.startTime, pieceInfo.endTime);
    }

    printf("\n");
}

int main()
{
    //从文件中读取json文件转换为结构体
    std::string configPath = CMAKE_CURRENT_SOURCE_DIR;
    configPath += "/test05/test_input.json";

    json testjson;
    std::ifstream jfile(configPath.c_str());
    jfile >> testjson;
    VideoInfo vi = testjson.at("output");
    printVideoInfo(vi);

    int32_t tilength = testjson["tracks"].size();
    TrackInfo ti[tilength];
    for (int32_t i = 0; i < tilength; i++)
    {
        printTrackInfo(testjson["tracks"][i]);
    }

    //将结构体转换为json文件
    VideoInfo srcVideo{ 12, 35, 11, 46 };
    json videoJson = srcVideo;
    std::cout << videoJson << std::endl;

    TrackInfo track;
    track.name = "hello";
    track.pieces[0] = { "aaaaa", 11, 22 };
    track.pieces[1] = { "bbbbb", 33, 44 };
    track.pieces[2] = { "ccccc", 55, 66 };
    track.pieces[3] = { "ddddd", 77, 88 };
    track.size = 4;

    json trackJson = track;
    std::cout << std::endl;
    std::cout << trackJson << std::endl;

    return 0;
}