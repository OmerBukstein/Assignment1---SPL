#include "Playlist.h"
#include "AudioTrack.h"
#include <iostream>
#include <algorithm>
Playlist::Playlist(const std::string& name) 
    : head(nullptr), playlist_name(name), track_count(0) {
    std::cout << "Created playlist: " << name << std::endl;
}
// TODO: Fix memory leaks!
// Students must fix this in Phase 1
Playlist::~Playlist() {
    #ifdef DEBUG
    std::cout << "Destroying playlist: " << playlist_name << std::endl;
    #endif
    PlaylistNode* current = head;
    // Removes all nodes from playlist and their data (pointer to track)
    while (current) {
        PlaylistNode* temp = current;
        current = current->next;
        delete temp->track;
        delete temp;
    }
}

//Copy Constructor
Playlist::Playlist(const Playlist& other_playlist): head(nullptr), playlist_name(other_playlist.playlist_name), track_count(other_playlist.track_count){
    if (other_playlist.head){
        head = new PlaylistNode(other_playlist.head->track->clone().release()); //need to add here clone for AudioTrack - initializing the head
        PlaylistNode* current = head;
        for (PlaylistNode* link = other_playlist.head->next; link != nullptr; link = link->next){
              PlaylistNode* new_node = new PlaylistNode(link->track->clone().release()); //need to add here clone for AudioTrack - copy the rest of the nodes
              current->next = new_node;
              current = current->next;
        }   
    }
    std::cout << "Created copy of playlist: " << other_playlist.playlist_name << std::endl;
}

//Copy Assignment operator
Playlist& Playlist::operator=(const Playlist& other_playlist){
    if (this != &other_playlist){
        //clearing the playlist linkedlist
        PlaylistNode* current = head;
        while (current) {
            PlaylistNode* temp = current;
            current = current->next;
            delete temp->track;
            delete temp;
        }
        head = nullptr;
        //making deep copy from other_playlist to this playlist
        playlist_name = other_playlist.playlist_name;
        track_count = 0; //each track that enters the playlist, we will add 1 to this field

        //copying the playlists
        PlaylistNode* other_head = other_playlist.head;
        PlaylistNode* my_head = nullptr;
        while(other_head){ //goes over other playlist, and creats simultaneously 
            AudioTrack* copied_track = other_head->track->clone().release();//do here the clone for other node - need to build in MP3 and WAV the clone function
            PlaylistNode* new_node = new PlaylistNode(copied_track); 
            if (!head){ //if head is nullptr
                head = new_node;
                my_head = head;
            } else{
                my_head->next = new_node;
                my_head = my_head->next;
            } 
            other_head = other_head->next;
            track_count++; 
        }
    }
    return *this;
}

void Playlist::add_track(AudioTrack* track) {
    if (!track) {
        std::cout << "[Error] Cannot add null track to playlist" << std::endl;
        return;
    }

    // Create new node - this allocates memory!
    PlaylistNode* new_node = new PlaylistNode(track);

    // Add to front of list
    new_node->next = head;
    head = new_node;
    track_count++;

    /*// Add to the back of list
    if (!head){
        head = new_node;
    } else {
        PlaylistNode* current = head;
        while (current->next){
            current = current -> next;
        }
        current->next = new_node;
    }
    track_count++;*/

    std::cout << "Added '" << track->get_title() << "' to playlist '" 
              << playlist_name << "'" << std::endl;
}

void Playlist::remove_track(const std::string& title) {
    PlaylistNode* current = head;
    PlaylistNode* prev = nullptr;

    // Find the track to remove
    while (current && current->track->get_title() != title) {
        prev = current;
        current = current->next;
    }

    if (current) {
        // Remove from linked list
        if (prev) {
            prev->next = current->next;
        } else {
            head = current->next;
        }
        // Realses the memory of the node and node memory (track)
        delete current->track;
        delete current;
        //Added on phase 1

        track_count--;
        std::cout << "Removed '" << title << "' from playlist" << std::endl;

    } else {
        std::cout << "Track '" << title << "' not found in playlist" << std::endl;
    }
}

void Playlist::display() const {
    std::cout << "\n=== Playlist: " << playlist_name << " ===" << std::endl;
    std::cout << "Track count: " << track_count << std::endl;

    PlaylistNode* current = head;
    int index = 1;

    while (current) {
        std::vector<std::string> artists = current->track->get_artists();
        std::string artist_list;

        std::for_each(artists.begin(), artists.end(), [&](const std::string& artist) {
            if (!artist_list.empty()) {
                artist_list += ", ";
            }
            artist_list += artist;
        });

        AudioTrack* track = current->track;
        std::cout << index << ". " << track->get_title() 
                  << " by " << artist_list
                  << " (" << track->get_duration() << "s, " 
                  << track->get_bpm() << " BPM)" << std::endl;
        current = current->next;
        index++;
    }

    if (track_count == 0) {
        std::cout << "(Empty playlist)" << std::endl;
    }
    std::cout << "========================\n" << std::endl;
}

AudioTrack* Playlist::find_track(const std::string& title) const {
    PlaylistNode* current = head;

    while (current) {
        if (current->track->get_title() == title) {
            return current->track;
        }
        current = current->next;
    }

    return nullptr;
}

int Playlist::get_total_duration() const {
    int total = 0;
    PlaylistNode* current = head;

    while (current) {
        total += current->track->get_duration();
        current = current->next;
    }

    return total;
}

std::vector<AudioTrack*> Playlist::getTracks() const {
    std::vector<AudioTrack*> tracks;
    PlaylistNode* current = head;
    while (current) {
        if (current->track)
            tracks.push_back(current->track);
        current = current->next;
    }
    return tracks;
}