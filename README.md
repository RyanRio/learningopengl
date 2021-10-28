# My Progression Through [learnopengl](https://learnopengl.com)

This repo will be divided into branches to reflect the tutorials on learnopengl. Main will always represent the most recent completed tutorial. I start recording history with the [shaders] branch. Currently, main represents the completed [Textures](https://learnopengl.com/Getting-started/Textures) guide. If I made any noteworthy changes I provide some brief notes in the 'Extra Notes' section, but then also elaborate more in the individual sections. My goal with this is to track my own learning but at the same time incorporate outside well-respected techniques into this tutorial, so that at the end of it I am not overwhelmed.

|Guide           |Branch          |Details                                                      |Extra Notes
|:-----------    |:-----------    |:----------                                                  |:----------
|[Hello Triangle]|[main]          | Nice Starting tutorial                                      |
|[Shaders]       |[shaders]       | Starter tutorial for how to make a triangle and rectangle   |Made it simpler to create a rectangle/triangle and choose between the two
|[Textures]      |[textures]      | Rendering textures on on the rectangle                      |I implemented textures using [KTX] texture features

[Hello Triangle]: https://learnopengl.com/Getting-started/Hello-Triangle "Hello Triangle Guide"
[Shaders]: https://learnopengl.com/Getting-started/Shaders "Shaders guide"
[Textures]: https://learnopengl.com/Getting-started/Textures

[main]: https://github.com/RyanRio/learningopengl/tree/main "main branch"
[shaders]: https://github.com/RyanRio/learningopengl/tree/shaders "/tree/shaders"
[textures]: https://github.com/RyanRio/learningopengl/tree/textures "/tree/textures"

[KTX]: https://github.com/KhronosGroup/KTX-Software

## Textures
A more in-depth look at my experience through the textures tutorial

#### Successes!
I was able to do some pretty cool stuff in this tutorial, and also started to gain an understanding that OpenGL could be much more flexible than I initially thought, i.e. you can go a long way by just using the API as documented, even if I wish the API was significantly more verbose at times, for learning sake. I was able to convert my jpg and png textures into a compressed (transcodeable format - tried both basislz and uasct) .ktx texture, and then render it using the corresponding extensions!

#### Trials and Tribulations
This took me quite awhile to get to somewhere that I felt happy with, because I went on quite the deep dive into the world of textures and texture compression. This was also my first run-in with opengl extensions, which does not function in the most obvious way. Additionally, GLAD helped in that it obscured some of the OpenGL monotony of platform-dependent function loading/etc, however.. it also obscured some of the OpenGL monotony :) Meaning for instance I could check if I had the extension *defined* in my `glad.h`, but that didn't necessarily say anything about whether it was actually supported on my machine, and I am still a bit confused about what extensions you are supposed to actually `require` in a `.vert` or `.frag`, as opposed to just *using* and *hoping* it works. A moment of extreme reassurance was given to me by:
`glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);`
Through which I was able to see that the internal format of my texture **was** in fact the same as one added by the extension I was using that supports `BC7`.
