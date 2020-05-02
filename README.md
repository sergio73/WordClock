# WordClock
This was a clock that I made as a gift. It shows the time with words instead of numbers, and once in a year, when her birthday comes, turns on the words "Feliz cumpleaños" (happy birthday) and it bright all day while it is doing a rainbow animation, until the day ends and everything come back to normal.

Anyone is free to copy the code and use it in your project. If you have doubts, don´t hesitate in contact with me! :)

## Final result
![alt text](https://github.com/sergio73/WordClock/blob/master/Images/WP_20160220_20_21_15_Pro.jpg)




## The panel
The first thing to do in this project was create the front panel, I based my design in the ones of this page: http://www.qlocktwo.com/Q2sprachen.php?lang=en it has multiple languages, so it can be useful for you.
I had to search for a monospaced font, because all the letters must have the same width & height. Also, it couldn´t be a normal font, because in some letters like the "O" the centre will fall when the laser cut the border, it had to have some union to avoid that. I don´t remember the font that I used, but I recommend you to get a good font, because it is the most important thing when it comes to look good.

![alt text](https://github.com/sergio73/WordClock/blob/master/Images/IMG-20160112-WA0001.jpg)

## The lights
After the panel was in home, I measured the size of the panel and bought a wood board of the same size. Then I measured each letter and did with a pencil the layout. After that I paste the RGB leds that I bought in Aliexpress. Although it may be more expensive than buying individual leds and resistances, I prefer to do it in this way, it is easier and faster. The bad part is that is more difficult to repair.

![alt text](https://github.com/sergio73/WordClock/blob/master/Images/IMG-20160123-WA0003.jpg)
![alt text](https://github.com/sergio73/WordClock/blob/master/Images/IMG-20160123-WA0005.jpg)

## The separation
I forgot to take one photo of the process when I put a semi translucent plastic above each word to diffuse the light, but you can image. I bought a semi translucent white plastic in eBay, cut with the dimensions of each word, even the ones that are lightless, and paste it  with superglue.

Now that the lights are installed and work properly, and the plastic is installed to, I must isolate each word to avoid light leaks. To do this, I used black silicon and cardboard, this is the worst process of all, you should be very carefully in order to not stain the front panel with the silicon.

![alt text](https://github.com/sergio73/WordClock/blob/master/Images/IMG-20160117-WA0001.jpg)

After isolate, each word I have to put more silicon because now I need to paste the board with the lights above this one.
You can see it here with all the lights on and without light leaks. 

![alt text](https://github.com/sergio73/WordClock/blob/master/Images/WP_20160123_19_05_47_Pro.jpg)

## The wiring
For the wiring, I used an AtMega328, the same that use Arduino ONE. I use only the microchip because I wanted to be small, and although there are other small Arduinos that may work as well, I also wanted to do it bare board. I don´t have any scheme of the connection that I did, but you can ask me if you have doubts. For the connection with the screen I used some old IDE cable that I had laying around.

This was at the begging of the programming, with an USB ASP to upload the code.
![alt text](https://github.com/sergio73/WordClock/blob/master/Images/IMG-20160129-WA0005.jpg)

Here you can see the project mounted on the final board, the board with the battery is the clock, a DS3231. If you are going to use this also, I recommend you strongly to cut the charge circuit of the board, and put a normal battery instead of one rechargeable. I tell you this because I didn´t do this at first, and I had to change the DS3231 because the battery was leaking acid and the DS3231 get broken.

![alt text](https://github.com/sergio73/WordClock/blob/master/Images/IMG-20160213-WA0001.jpg)

And here is everything mounted on the board. I didn´t put any process of the framework because I don´t have the tools for that and I went to a carpenter to get the work done.

Again, here is the clock finished, and a close look at the buttons.
![alt text](https://github.com/sergio73/WordClock/blob/master/Images/WP_20160220_20_21_15_Pro.jpg)
![alt text](https://github.com/sergio73/WordClock/blob/master/Images/IMG-20160220-WA0001.jpg)
