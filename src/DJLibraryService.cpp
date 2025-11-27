#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}

//Destructor - removes all pointers of track from vector
DJLibraryService::~DJLibraryService(){
    for (AudioTrack* track : library){
        delete track;
    }
}

//Copy constructor - makes deep copy to other DJ library 
DJLibraryService::DJLibraryService(const DJLibraryService& other): playlist(other.playlist), library(){
    if (!other.library.empty()){ 
        for (AudioTrack* track : other.library){
            if (track != nullptr){ //checks that we will not make clone on nullptr track
                library.push_back(track->clone().release());
            }            
        }
    }
}

//Copy assignment operator for this*
DJLibraryService& DJLibraryService::operator=(const DJLibraryService& other){
    if (this != &other){
        for (AudioTrack* track : library){
            delete track;
        }
        library.clear(); //library is a vector
        playlist = other.playlist;
        for (AudioTrack* track : other.library){
            if (track != nullptr){ //checks that we will not make clone on nullptr track
                library.push_back(track->clone().release());
            }            
        }
    }
    return *this;
}

    /**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for(SessionConfig::TrackInfo trackInfo: library_tracks){
        if(trackInfo.type=="MP3"){
            AudioTrack* track=new MP3Track(
                trackInfo.title,
                trackInfo.artists,
                trackInfo.duration_seconds,
                trackInfo.bpm,
                trackInfo.extra_param1,
                trackInfo.extra_param2
            ); //Log creation message was made in MP3 constructor
            library.push_back(track);
        }
        else{
            if(trackInfo.type=="WAV"){
            AudioTrack* track=new WAVTrack(
                trackInfo.title,
                trackInfo.artists,
                trackInfo.duration_seconds,
                trackInfo.bpm,
                trackInfo.extra_param1,
                trackInfo.extra_param2
            );//Log creation message was made in MP3 constructor
            library.push_back(track);
        }
      }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    this->playlist=Playlist(playlist_name);//activate assignemnt operator and getting rid of previous playlist
    for(int index: track_indices){
        if(index < 1|| index > static_cast<int> (library.size())){ //library.size returns long type and index is int
         std::cout << "[WARNING] Invalid track index: " << index << std::endl;
        }
        else{
            AudioTrack* track=library[index-1];
            AudioTrack* track_clone=track->clone().release();
            if(!track_clone){
              std::cout << "[ERROR] Failed to clone track: " << track->get_title() << std::endl;
            }
            else{
                track_clone->load();
                track_clone->analyze_beatgrid();
                playlist.add_track(track_clone); //Log: Added ’<title>’ to playlist ’<playlist_name>’ is printed from add_track - playlist class
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << playlist.get_track_count() << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> track_titles;
    for (AudioTrack* current_track : playlist.getTracks())
    {
        track_titles.push_back(current_track->get_title());
    }
    return track_titles;
}
