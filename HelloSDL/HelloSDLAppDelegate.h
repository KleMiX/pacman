//
//  HelloSDLAppDelegate.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/17/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface HelloSDLAppDelegate : NSObject <NSApplicationDelegate> {
@private
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
