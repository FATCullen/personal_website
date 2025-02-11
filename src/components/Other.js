import React, { useState, useEffect, useCallback, useRef } from 'react';
import judo from "../images/judo.png";
import chess from "../images/chess.png";
import uwo from "../images/uwo.png";

import bad_sackingen from "../images/travel/bad_sackingen.jpg";
import hawaii from "../images/travel/hawaii.jpg";
import idaho from "../images/travel/idaho.jpg";
import ireland from "../images/travel/ireland.jpg";
import lech from "../images/travel/lech.jpg";
import monaco from "../images/travel/monaco.jpg";
import monaco_hairpin from "../images/travel/monaco_hairpin.jpg";
import nice from "../images/travel/nice.jpg";
import saint_christoph from "../images/travel/saint_christoph.jpg";
import switzerland from "../images/travel/switzerland.jpg";
import vienna from "../images/travel/vienna.jpg";
import zaanse_schans from "../images/travel/zaanse_schans.jpg";

export default function Other() {
  const travelImages = [bad_sackingen,hawaii,idaho,ireland,lech,monaco,monaco_hairpin,nice,saint_christoph,switzerland,vienna,zaanse_schans];
  const travelCaptions = [
    "Bad Säckingen, on the Swiss - German Border",
    "Waterfall in Hawaii",
    "Snake River, Idaho",
    "Gap of Dunloe, Ireland",
    "Lech am Arlberg, Austria",
    "Monaco Harbour at Night",
    "Monaco Hairpin",
    "Nice, France",
    "Saint Christoph, Austria",
    "Wakeboarding in Switzerland",
    "Belvedere Palace, Vienna",
    "Zaanse Schans, Just Outside Amsterdam"
  ];

  const [currentImage, setCurrentImage] = useState(0);
  const [isTransitioning, setIsTransitioning] = useState(false);
  const intervalRef = useRef(null); // Store interval ID

  // Function to start the auto-switch timer
  const startTimer = useCallback(() => {
    clearInterval(intervalRef.current); // Clear existing timer
    intervalRef.current = setInterval(() => {
      setCurrentImage((prev) => (prev + 1) % travelImages.length);
    }, 5000);
  }, [travelImages.length]);

  // Function to handle next image
  const nextImage = useCallback(() => {
    setIsTransitioning(true);
    setCurrentImage((prev) => (prev + 1) % travelImages.length);
    startTimer(); // Reset timer when user clicks
  }, [travelImages.length, startTimer]);

  // Function to handle previous image
  const prevImage = () => {
    setIsTransitioning(true);
    setCurrentImage((prev) => (prev - 1 + travelImages.length) % travelImages.length);
    startTimer(); // Reset timer when user clicks
  };

  // Start the auto-switch timer on mount
  useEffect(() => {
    startTimer();
    return () => clearInterval(intervalRef.current); // Cleanup on unmount
  }, [startTimer]);

  const handleTransitionEnd = () => {
    setIsTransitioning(false);
  };

  return (
    <div className="max-w-6xl mx-auto px-4 py-8">
      <h1 className="text-3xl md:text-4xl font-bold mb-8 text-white">Other</h1>

      <div className="space-y-8">
         {/* UWO */}
         <div className="bg-gray-800 rounded-lg p-6 flex flex-col md:flex-row items-center">
          <div className="flex-1 md:pr-6 text-center md:text-left">
            <h2 className="text-xl font-semibold text-white">UW Orbital</h2>
            <p className="text-gray-300 mb-2">
              I am a member of UWaterloo Orbital, my university's satellite design team.  
              We're currently in the process of building our Cube Sat for CSDC 7.
              I work on the firmware and groundstation subteams, and currently I am focusing on implementing doppler effect correction algorithms for LEO communications.
            </p>
          </div>
          <div className="w-full md:w-2/5 mt-4 md:mt-0">
            <img src={uwo} alt="Me doing Judo" className="w-full h-auto object-contain" />
          </div>
        </div>

        {/* Judo */}
        <div className="bg-gray-800 rounded-lg p-6 flex flex-col md:flex-row items-center">
          <div className="flex-1 md:pr-6 text-center md:text-left">
            <h2 className="text-xl font-semibold text-white">Judo</h2>
            <p className="text-gray-300 mb-2">
              For the past 7 years I have been training and competing in Judo, and currently have my second degree black belt (Nidan).  
              Prior to moving to Ontario for school I was a Provincial A level competitor with Team BC, and a carded member of the Provincial development program.
              I've had the opportunity to compete at a variety of competitions, including Canadian and US Nationals, as well as places as far away as Tahiti, England, Whales, and Switzerland.
              Currently I am training at Asahi Judo Club in Waterloo, and (when my classes don't interfere) I'm helping out at the Waterloo University Judo Club.
            </p>
          </div>
          <div className="w-full md:w-2/5 mt-4 md:mt-0">
            <img src={judo} alt="Me doing Judo" className="w-full h-auto object-contain" />
          </div>
        </div>

        {/* Chess */}
        <div className="bg-gray-800 rounded-lg p-6 flex flex-col md:flex-row items-center">
          <div className="flex-1 md:pr-6 text-center md:text-left">
            <h2 className="text-xl font-semibold text-white">Chess</h2>
            <p className="text-gray-300 mb-2">
              While I'm not necessarily very good at it, chess is something I've put a lot of time into and really enjoy.
              I started playing during the pandemic and quickly became hooked.
              Overall my chess journey has included me founding and leading the first chess club at my high school, building an interactive website to help teach new club members how to play, and building my own chess AI and engine (see my projects section).
              Feel free to add me on Chess.com and start up a daily game!
            </p>
          </div>
          <div className="w-full md:w-2/5 mt-4 md:mt-0">
            <img src={chess} alt="Chess board" className="w-full h-auto object-contain" />
          </div>
        </div>

        {/* Travel Section */}
        <div className="bg-gray-800 rounded-lg p-6 flex flex-col md:flex-row items-center">
          <div className="flex-1 md:pr-6 text-center md:text-left">
            <h2 className="text-xl font-semibold text-white">Travel Photos</h2>
            <p className="text-gray-300 mb-2">
              Just a place to share some photos of the places I've been around the world.
            </p>
          </div>
          <div className="w-full md:w-2/5 mt-4 md:mt-0">
            <div className="relative group overflow-hidden rounded-lg">
              <div 
                className="flex transition-transform duration-500 ease-in-out"
                style={{ transform: `translateX(-${currentImage * 100}%)` }}
                onTransitionEnd={handleTransitionEnd}
              >
                {travelImages.map((img, index) => (
                  <img 
                    key={index}
                    src={img} 
                    alt={travelCaptions[index]} 
                    className="w-full h-auto object-cover flex-shrink-0"
                  />
                ))}
              </div>
              <div className="absolute inset-0 flex items-center justify-between opacity-0 group-hover:opacity-100 transition-opacity px-4">
                <button 
                  onClick={prevImage} 
                  className="p-2 bg-black/50 text-white rounded-full hover:bg-black/70 transition-colors"
                  disabled={isTransitioning}
                >
                  ←
                </button>
                <button 
                  onClick={nextImage} 
                  className="p-2 bg-black/50 text-white rounded-full hover:bg-black/70 transition-colors"
                  disabled={isTransitioning}
                >
                  →
                </button>
              </div>
            </div>
            {/* Caption below the image */}
            <p className="text-center text-gray-300 mt-2">{travelCaptions[currentImage]}</p>
          </div>
        </div>
      </div>
    </div>
  );
}