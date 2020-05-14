#include <stdio.h>
#include <MIDIData.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
	MIDIData* pMIDIData;
	MIDITrack* pMIDITrack;
	/* MIDIデータの生成(フォーマット0,トラック数1,TPQNベース,120) */
	pMIDIData = MIDIData_Create(MIDIDATA_FORMAT0, 1, MIDIDATA_TPQNBASE, 120);
	if (pMIDIData == NULL) {
		printf("MIDIデータの生成に失敗しました。\n");
		return 0;
	}


	std::ifstream ifs("output.txt");
	std::string str;
	if (ifs.fail())
	{
		std::cerr << "失敗" << std::endl;
		return -1;
	}


	/* 最初のトラックへのポインタを取得 */
	pMIDITrack = MIDIData_GetFirstTrack(pMIDIData);
	/* イベントを挿入 */
	MIDITrack_InsertTrackName(pMIDITrack, 0, L"ちょうちょ"); /* タイトル */
	MIDITrack_InsertTempo(pMIDITrack, 0, 60000000 / 120); /* 120BPM */
	MIDITrack_InsertProgramChange(pMIDITrack, 0, 0, 0); /* Piano1 */







	while (getline(ifs, str))
	{
		//std::cout << "[" << str << "]" << std::endl;
		
		//時間をパース
		string time_str;
		time_str = str;
		time_str.erase(str.find("ms"));
		double time = atof(time_str.c_str())*250;

		str.erase(0, str.find("ms")+2);
		
		string buff;
		vector <int> eve;
		vector <int> key;
		string event_str;
		int event;

		for (;str[0] == '-';) {

			str.erase(0,1);

			event_str = str[0];
			eve.push_back(atoi(event_str.c_str()));
			str.erase(0,1);


			event_str = str;
			event_str.erase(2, str.length() - 1);
			key.push_back(atoi(event_str.c_str())+21);
			str.erase(0,2);

		}



		for (int i = 0; i < eve.size(); i++) {
			if (eve[i] == 1) {
				/* ノートイベントを挿入            Time  ch  key  vel  dur */
				MIDITrack_InsertNoteOn(pMIDITrack, time, 0, key[i], 100); 
				cout << "[on][" << to_string(time) << "s][" << to_string(key[i]) << "]" << endl;
			}
			else if (eve[i] == 0) {
				MIDITrack_InsertNoteOff(pMIDITrack, time, 0, key[i], 100);
				cout << "[off][" << to_string(time) << "s][" << to_string(key[i]) << "]" << endl;
			}
		}
	}


	/* エンドオブトラックイベントを挿入 */
	MIDITrack_InsertEndofTrack(pMIDITrack, 1920);
	/* MIDIデータを保存 */
	MIDIData_SaveAsSMF(pMIDIData, L"chocho.mid");
	/* MIDIデータをメモリ上から削除 */
	MIDIData_Delete(pMIDIData);
	pMIDIData = NULL;
	return 1;
}