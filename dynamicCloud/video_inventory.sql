CREATE TABLE 'video_inventory' (
 `id` int(11) NOT NULL AUTO_INCREMENT,
 `name` varchar(255) NOT NULL,				-- string
 `category` varchar(255) NOT NULL,			-- string
 `length` int(11) NOT NULL,					-- number
 `rented` tinyint(1) NOT NULL DEFAULT '0',	-- radio
 `subtilte` tinyint(1) NOT NULL DEFAULT '0', -- check box
 ''
 PRIMARY KEY (`id`),
 UNIQUE KEY `uniqueName` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=latin1