CREATE TABLE document_status (
    status_id INT PRIMARY KEY AUTO_INCREMENT,
    doc_id INT NOT NULL,
    status ENUM('draft', 'pending', 'approved', 'rejected'),
    changed_by INT,                    -- 操作人ID
    changed_at DATETIME DEFAULT NOW(),
    FOREIGN KEY (doc_id) REFERENCES documents(doc_id)
);
