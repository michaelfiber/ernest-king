# Things maybe worth sharing during the jam:

## Canvas Sizing
This will make the canvas fill whatever window size it gets without stretching or getting cutoff.  If the ratio is different it'll get black bars. If you set its size in itch.io the same as the game res then you shouldn't see the black background of this page there.

```
<style>
    * {
        margin: 0;
        padding: 0;
    }
    body {
        background-color: black;
        width: 100vw;
        height: 100vh;
        overflow: hidden;
    }
    canvas.emscripten { 
        display: block;
        object-fit: contain;
        width: 100vw;
        height: 100vh;
        image-rendering: pixelated;
    }
</style>
```