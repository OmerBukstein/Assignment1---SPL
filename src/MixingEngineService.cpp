#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    :decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0]=nullptr;
    decks[1]=nullptr;
    std::cout<<"[MixingEngineService] Initialized with 2 empty decks."<< std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout<<"[MixingEngineService] Cleaning up decks...."<< std::endl;
    if(decks[0]!=nullptr){
        delete decks[0];
        decks[0]=nullptr;
    }
     if(decks[1]!=nullptr){
        delete decks[1];
        decks[1]=nullptr;
    }

}

/**
 * Implement of copy constructor to MixingEngineService object
 */
MixingEngineService::MixingEngineService(const MixingEngineService &other_mixer)
    :active_deck(other_mixer.active_deck),auto_sync(other_mixer.auto_sync),bpm_tolerance(other_mixer.bpm_tolerance)
{
    if(other_mixer.decks[0]!=nullptr){
        decks[0]=other_mixer.decks[0]->clone().release();
    }
    else{
        decks[0]=nullptr;
    }
    if(other_mixer.decks[1]!=nullptr){
        decks[1]=other_mixer.decks[1]->clone().release();
    }
    else{
        decks[1]=nullptr;
    }
}

/**
 * Implement of copy assignment operator to MixingEngineService object
 */

MixingEngineService& MixingEngineService::operator=(const MixingEngineService& other_mixer) {
    if (this == &other_mixer) {
        return *this;
    }
    for (int i = 0; i < 2; ++i) {
        if (decks[i] != nullptr) {
            delete decks[i];    
            decks[i] = nullptr; 
        }
    }
    active_deck = other_mixer.active_deck;
    auto_sync = other_mixer.auto_sync;
    bpm_tolerance = other_mixer.bpm_tolerance;
    for (int i = 0; i < 2; ++i) {
        if (other_mixer.decks[i] != nullptr) {
            decks[i] = other_mixer.decks[i]->clone().release();
        } else {
            decks[i] = nullptr;
        }
    }
    return *this;
}

/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout<<"\n=== Loading Track to Deck ==="<< std::endl;
    PointerWrapper<AudioTrack> track_clone=track.clone();
    if(!track_clone){
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }
    size_t target_index;
    if(decks[0]==nullptr && decks[1]==nullptr){
        target_index=0;
    }
    else{
        target_index=1-active_deck;
    }
    std::cout << "[Deck Switch] Target deck: " << target_index << std::endl;
    if(decks[target_index]!=nullptr){
        delete decks[target_index];
        decks[target_index]=nullptr;
    }
        

    //g
    track_clone->load();
    track_clone->analyze_beatgrid();
    

    //h
    if((decks[0]!=nullptr || decks[1]!=nullptr) && auto_sync){//in order to mix we need one of them to be active
        if(!can_mix_tracks(track_clone)){
            sync_bpm(track_clone);
        }
    }

    //i
    decks[target_index]=track_clone.release();
    std::cout << "[Load Complete] '" << decks[target_index]->get_title() << "' is now loaded on deck " << target_index << std::endl;

    //j
    if(decks[active_deck]!=nullptr && active_deck!=target_index){
        std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck]=nullptr;
    }

    //k
    active_deck=target_index;
    
    //l
    std::cout << "[Active Deck] Switched to deck " << target_index << std::endl;
    //m
    return target_index;
}


/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if (decks[active_deck] == nullptr){ //active deck can't be nullptr - it means without track
        return false;
    }
    if (!track){ //as well about track, checks if tracks is existed
        return false;
    }
    int active_bpm = decks[active_deck]->get_bpm();
    int other_bpm = track->get_bpm();
    if (other_bpm>=active_bpm){
        return (other_bpm-active_bpm)<= bpm_tolerance; //get both bpm's and check if ther absulute sub is less the bpm_tolerance
    }
    else{
        return (active_bpm-other_bpm)<=bpm_tolerance;
    }
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if (track && decks[active_deck] != nullptr){ //check if both decks will not be nullptr
        int original_bpm = track->get_bpm();
        int avg = original_bpm + decks[active_deck]->get_bpm(); //calculates the avg, and reduces track's bpm 
        avg = avg/2; 
        track->set_bpm(avg);
        std::cout << "[Sync BPM] Syncing BPM from " << original_bpm << " to " << track->get_bpm() << std::endl;
    }
}
