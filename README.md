# How I Trolled keyloggers

![image](https://user-images.githubusercontent.com/60795188/181312338-8189f4df-6aa7-4e4a-ad24-82fb4973f68f.png)

## Introduction

* Keyloggers is an archaic method used by malware authors its basically record everything you type on a keyboard.

## Deep analysis

* the API we are hacking is ```GetAsyncKeyState``` , its one of the most common APIs that used by Keyloggers ,it Determines whether a key is up or down at the time the function is called ,it takes ```int``` in arguments, and returns ```SHORT```

* as you can see  ```GetAsyncKeyState``` is just a wrapper around  ```ntUserGetAsyncKeyState```

    ![image](https://user-images.githubusercontent.com/60795188/181027573-b0047600-9204-48fb-a871-cc6decdc48cc.png)
 
 * ```ntUserGetAsyncKeyState```
 
    ![image](https://user-images.githubusercontent.com/60795188/181042793-332fd5e8-d49b-490a-9732-903fa7385133.png)

* if you perform a ```GetAsyncKeyState``` the execution will be like this :

    ![Untitled](https://user-images.githubusercontent.com/60795188/181153111-9f94e5d5-eda1-4a60-badb-285f6b961123.png)
 
     ### Notes
   * The real code of ```ntUserGetAsyncKeyState``` function runs in Kernel Mode.
   * When a user-mode application calls the Nt or Zw version of a native system services routine, the routine always treats the parameters that it receives as values that come from a user-mode source that is not trusted .
  
## Patching GetAsyncKeyState :
 
 * first thing we do is locate ```GetAsyncKeyState``` in memory then we overwrite the first 12 bytes and make it jump to our fake function .
 
    ![image](https://user-images.githubusercontent.com/60795188/181050738-c179511f-1a38-499d-a776-2d09bc2ceae0.png)
    
 * if the specified key is down/pressed ```GetAsyncKeyState``` returns 0x8001 or -32767 in decimal to abuse it ,we create an array of characters of our choice and return -32767 in every character so the keylogger log it sequentially ,then we returns 0 everytime ```GetAsyncKeyState``` get called so the keylogger get blocked from logging .
  
      ```C
      unsigned char	msg[] = {
      'T','R','O','L','L','E','D'
      };
    ```

    ![image](https://user-images.githubusercontent.com/60795188/181054594-cea62c51-70f0-4aa7-bda1-85c4cf53f021.png)
    
 * our function in memory:

    ![image](https://user-images.githubusercontent.com/60795188/181062115-b33354e2-68d6-4aa0-b7e6-6506d6be2bc4.png)

### Result


https://user-images.githubusercontent.com/60795188/181157761-695dec5c-3559-4d85-b59e-5ddc83a5bf64.mp4

![we-do-a-little-trolling](https://user-images.githubusercontent.com/60795188/160684587-14c40393-24f2-441e-803d-62104b864680.gif)

### Lastly

* im planning to make a highly effective program that detect all ( high/low ) level and both direct/indirect syscalls keyloggers and shut them down .
