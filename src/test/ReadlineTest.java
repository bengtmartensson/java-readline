/**************************************************************************
/* ReadlineTest.java -- Test program for the Java wrapper of GNU readline
/*
/* Java Wrapper Copyright (c) 1998 by Bernhard Bablok (bablokb@gmx.de)
/*
/* This sample program is placed into the public domain and can be
/* used or modified without any restriction.
/*
/* This program is distributed in the hope that it will be useful, but
/* WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
/**************************************************************************/

/**
 * ReadlineTest.java
 * 
 * This class shows the usage of the readline wrapper. It will read lines 
 * from standard input using the GNU-Readline library. You can use the
 * standard line editing keys. You can also define application specific 
 * keys. Put this into your ~/.inputrc (or into whatever file $INPUTRC
 * points to) and see what happens if you press function keys F1 to F3:
 * <pre>
 *$if ReadlineTest
 *"\e[11~":	"linux is great"
 *"\e[12~":	"jikes is cool"
 *"\e[13~":	"javac is slow"
 *$endif
 *</pre>
 *
 * @author $Author: bablokb $
 * @version $Revision: 1.2 $
 */

import java.io.*;
import gnu.readline.*;

public class ReadlineTest {
  
  public ReadlineTest() {
    
  }

  public static void main(String[] args) {
    String line;
    Readline.initReadline("ReadlineTest");
    while (true) {
      try {
	line = Readline.readline("linux> ");
	if (line == null)
	  System.out.println("no input");
	else
	  System.out.println("line = >" + line + "<");
      } catch (IOException e) {
	System.out.println(e.toString());
	System.exit(0);
      }
    }
  }

}
