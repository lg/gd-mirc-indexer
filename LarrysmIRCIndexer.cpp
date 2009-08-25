#include <windows.h>
#include <string>

#include "LarrysmIRCIndexer.h"
#include "LarGDSPlugin.h"
#include "resource.h"

using namespace std;

HINSTANCE Instance;
const CLSID PluginClassID = {0x5b4de43e,0x8104,0x4a56,{0x84,0x68,0x44,0xd1,0x67,0xfb,0x5c,0x31}};
LarGDSPlugin GDSPlugin(PluginClassID);

int WINAPI DllMain(HINSTANCE hinstDLL, unsigned long dwReason, void *reserved) {
	Instance = hinstDLL;
	return 1;
}

bool RegisterComponentWithGDS(void) {
	// Generate path to icon
	char ModuleName[1024];
    ::GetModuleFileName(Instance, ModuleName, 1023);

	char ResourceNum[10];
	itoa(IDI_MIRC, ResourceNum, 10);

	string FullPath;
	FullPath = string(ModuleName) + "," + string(ResourceNum);

	// Register component
	return GDSPlugin.RegisterPlugin("Larry's mIRC Indexer 1.00 by Larry Gadea", "Indexes private messages from mIRC", FullPath);
}

bool UnregisterComponentWithGDS(void) {
	return GDSPlugin.UnregisterPlugin();
}

bool IndexMessage(char* RawData) {
	// RawData will come in format <MyName> <OtherPartyName> <Sender> <Message>

	string FullData(RawData);

	string MyName;
	string Sender;
	string OtherParty;
	string Message;
	size_t Space1 = 0;
	size_t Space2 = 0;
	size_t Space3 = 0;

	// Find all the space locations to be able to properly tokenize the raw data
	Space1 = FullData.find(" ", 0);
	if ((Space1 == FullData.npos) || (FullData.length() <= Space1 + 1)) {
		return false;
	}
	Space2 = FullData.find(" ", Space1 + 1);
	if ((Space2 == FullData.npos) || (FullData.length() <= Space2 + 1)) {
		return false;
	}
	Space3 = FullData.find(" ", Space2 + 1);
	if ((Space2 == FullData.npos) || (FullData.length() <= Space2 + 1)) {
		return false;
	} 

	// Assemble sender, other party, and message data into seperate variables
	MyName = FullData.substr(0, Space1);
	OtherParty = FullData.substr(Space1 + 1, Space2 - Space1 - 1);
	Sender = FullData.substr(Space2 + 1, Space3 - Space2 - 1);
	Message = FullData.substr(Space3 + 1);

	string Conversation;
	Conversation = Sender + ": " + Message;

	// Send the IM event and get GDS to index it. Also a ConversationID will be generated
	// if this is a new conversation.
	return GDSPlugin.SendIMEvent(Conversation, MyName, OtherParty, OtherParty, GDSPlugin.GetConversationID(OtherParty));;
}