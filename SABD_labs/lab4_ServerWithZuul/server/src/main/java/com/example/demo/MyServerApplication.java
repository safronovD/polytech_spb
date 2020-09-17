package com.example.demo;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.*;
import org.apache.commons.lang3.RandomStringUtils;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

@SpringBootApplication
public class MyServerApplication {

	public static void main(String[] args) {
		SpringApplication.run(MyServerApplication.class, args);
	}

	@RestController
	public class DataObjectController {
		@RequestMapping(value = "/data/{id}", method = RequestMethod.GET)
		@ResponseBody
		public DataObject findById(
				@PathVariable long id, HttpServletRequest req, HttpServletResponse res) {
			if (req.getHeader("Test") != null) {
				res.addHeader("Test", req.getHeader("Test"));
			}
			return new DataObject(id, RandomStringUtils.randomAlphanumeric(10));
		}
	}
}
