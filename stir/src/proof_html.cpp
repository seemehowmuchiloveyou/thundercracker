/* -*- mode: C; c-basic-offset: 4; intent-tabs-mode: nil -*-
 *
 * STIR -- Sifteo Tiled Image Reducer
 * M. Elizabeth Scott <beth@sifteo.com>
 *
 * Copyright <c> 2011 Sifteo, Inc. All rights reserved.
 */

/*
 * AUTOMATICALLY GENERATED by proof_html.py
 * Do not edit by hand!
 */

#include "proof.h"

const char *Stir::ProofWriter::header =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head> \n"
    "  <script> \n"
    " \n"
    "     /*\n"
    "      * Define a tile pool (per-group)\n"
    "      */\n"
    "\n"
    "     function defineTiles(prefix, tiles) {\n"
    "       for (var i = 0; i < tiles.length; i++) {\n"
    "\n"
    "         // Load it using a Data URI and anonymous img element\n"
    "         var img = new Image(8, 8);\n"
    "         img.src = prefix + tiles[i];\n"
    "\n"
    "         // Keep a list of closures that can be used to redraw\n"
    "         // every occurrance of this tile in the TileGrids.\n"
    "         img.tgRedraw = []\n"
    " \n"
    "        tiles[i] = img;\n"
    "       }\n"
    "       return tiles;\n"
    "     }\n"
    "\n"
    "     /*\n"
    "      * Object for a single TileGrid, with some interactive features.\n"
    "      * Renders onto an HTML5 Canvas\n"
    "      */\n"
    "\n"
    "     allTileGrids = [];\n"
    "     highlightTile = null;\n"
    "\n"
    "     function TileGrid(pool, canvasId) {\n"
    "       var obj = this;\n"
    "\n"
    "       allTileGrids.push(this);\n"
    "\n"
    "       this.pool = pool;\n"
    "       this.canvas = document.getElementById(canvasId);\n"
    "       this.ctx = this.canvas.getContext(\"2d\");\n"
    "       this.width = this.canvas.width / 8;\n"
    "       this.height = this.canvas.height / 8;\n"
    "\n"
    "       this.canvas.onmousemove = function(evt) {\n"
    "         setTileHighlight(obj.tileAt(Math.floor(mouseX(evt) / 8),\n"
    "                                     Math.floor(mouseY(evt) / 8)));\n"
    "       }\n"
    "\n"
    "       this.canvas.onmouseout = function(evt) {\n"
    "         setTileHighlight(null);\n"
    "       }\n"
    "     }\n"
    "\n"
    "     TileGrid.prototype.range = function(begin, end) {\n"
    "       var a = [];\n"
    "       for (var i = begin; i < end; i++)\n"
    "         a[i - begin] = i;\n"
    "       this.array(a);\n"
    "     }\n"
    "\n"
    "     TileGrid.prototype.array = function(a) {\n"
    "       this.tiles = a;\n"
    "     }\n"
    "\n"
    "     TileGrid.prototype.draw = function() {\n"
    "       /*\n"
    "        * Draw the whole TileGrid unconditionally\n"
    "        */\n"
    "\n"
    "       for (var y = 0; y < this.height; y++)\n"
    "         for (var x = 0; x < this.width; x++)\n"
    "           this.drawTile(x, y);\n"
    "     }\n"
    "\n"
    "     TileGrid.prototype.installDrawHandlers = function() {\n"
    "       /*\n"
    "        * Install tile redraw handlers for every tile in the TileGrid\n"
    "        */\n"
    "\n"
    "       for (var y = 0; y < this.height; y++)\n"
    "         for (var x = 0; x < this.width; x++)\n"
    "           var f = function (obj, x, y) {\n"
    "             var t = obj.tileAt(x, y);\n"
    "\n"
    "             if (t) {\n"
    "               t.tgRedraw.push(function() { obj.drawTile(x, y); })\n"
    "             }\n"
    "\n"
    "           } (this, x, y);\n"
    "     }\n"
    "\n"
    "     TileGrid.prototype.tileAt = function(x, y) {\n"
    "       /*\n"
    "        * Returns the tile at location (x,y), if any.\n"
    "        */\n"
    "\n"
    "       var i = x + y * this.width;\n"
    "       if (i < this.tiles.length)\n"
    "         return this.pool[this.tiles[i]];\n"
    "     }\n"
    "\n"
    "     TileGrid.prototype.drawTile = function(x, y) {\n"
    "       /*\n"
    "        * (Re)draw a single tile at a specified location on this grid.\n"
    "        */\n"
    "\n"
    "       var tile = this.tileAt(x, y);\n"
    "       if (tile) {\n"
    "\n"
    "         this.ctx.globalAlpha = 1.0;\n"
    "         this.ctx.drawImage(tile, x*8, y*8);\n"
    "\n"
    "         if (highlightTile != null && tile != highlightTile) {\n"
    "           // Dim this tile\n"
    "           this.ctx.globalAlpha = 0.5;\n"
    "           this.ctx.fillStyle = \"#000\";\n"
    "           this.ctx.fillRect(x*8, y*8, 8, 8);\n"
    "         }\n"
    "\n"
    "       } else {\n"
    " \n"
    "         // Background\n"
    "         this.ctx.globalAlpha = 1.0;\n"
    "         this.ctx.fillStyle = \"#000\";\n"
    "         this.ctx.fillRect(x*8, y*8, 8, 8);\n"
    "       }\n"
    "     }\n"
    "\n"
    "     function onload() {\n"
    "       /*\n"
    "        * Draw all TileGrids after our images have loaded\n"
    "        */\n"
    "\n"
    "       for (var i in allTileGrids) {\n"
    "         allTileGrids[i].installDrawHandlers();\n"
    "         allTileGrids[i].draw();\n"
    "       }\n"
    "     }\n"
    "\n"
    "     function drawTile(t) {\n"
    "       /*\n"
    "        * Redraw a single tile, on every grid.\n"
    "        */\n"
    "\n"
    "       if (t) {\n"
    "         for (var i in t.tgRedraw)\n"
    "           t.tgRedraw[i]();\n"
    "       }\n"
    "     }\n"
    "\n"
    "     function setTileHighlight(t) {\n"
    "       /*\n"
    "        * Change the tile that's currently highlighted.\n"
    "        */\n"
    "\n"
    "       if (t != highlightTile) {\n"
    "         var prev = highlightTile;\n"
    "         highlightTile = t;\n"
    "\n"
    "         if (prev == null || t == null) {\n"
    "           /*\n"
    "            * Transitions from highlighted to not highlighted or vice versa\n"
    "            * require a full draw, since we're dimming all non-hilighted tiles.\n"
    "            */\n"
    "          \n"
    "           for (var i in allTileGrids)\n"
    "             allTileGrids[i].draw();\n"
    "\n"
    "         } else {\n"
    "           /*\n"
    "            * Only draw the affected tiles\n"
    "            */\n"
    "\n"
    "           drawTile(prev);\n"
    "           drawTile(t);\n"
    "         }\n"
    "       }\n"
    "     }\n"
    "\n"
    "     function mouseX(evt) {\n"
    "       var x;\n"
    "\n"
    "       if (evt.pageX)\n"
    "         x = evt.pageX;\n"
    "       else\n"
    "         x = evt.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;\n"
    "\n"
    "       return x - evt.target.offsetLeft;\n"
    "     }\n"
    "\n"
    "     function mouseY(evt) {\n"
    "       var y;\n"
    "\n"
    "       if (evt.pageY)\n"
    "         y = evt.pageY;\n"
    "       else\n"
    "         y = evt.clientY + document.body.scrollTop + document.documentElement.scrollTop;\n"
    "\n"
    "       return y - evt.target.offsetTop;\n"
    "     }\n"
    "\n"
    "  </script> \n"
    "  <style> \n"
    " \n"
    "    body { \n"
    "      color: #eee; \n"
    "      background: #222; \n"
    "      font-family: verdana, tahoma, helvetica, arial, sans-serif; \n"
    "      font-size: 12px; \n"
    "      margin: 10px 5px 50px 5px; \n"
    "    } \n"
    "  \n"
    "    h1 { \n"
    "      background: #fff; \n"
    "      color: #222; \n"
    "      font-size: 22px; \n"
    "      font-weight: normal; \n"
    "      padding: 10px; \n"
    "      margin: 0; \n"
    "    } \n"
    " \n"
    "    h2 { \n"
    "      color: #fff; \n"
    "      font-size: 16px; \n"
    "      font-weight: normal; \n"
    "      padding: 10px; \n"
    "      margin: 0; \n"
    "    } \n"
    " \n"
    "    p { \n"
    "      padding: 0 10px; \n"
    "    } \n"
    "\n"
    "    canvas {\n"
    "      padding: 0;\n"
    "      margin: 0;\n"
    "    }\n"
    " \n"
    "  </style> \n"
    "</head> \n"
    "<body onload=\"onload()\">\n"
    "\n";
